#include "../organize.h"

volatile sig_atomic_t g_sig = 0;
// ─── handlers ────────────────────────────────────────────────────────────────

// Interactive prompt: Ctrl+C → new line, redisplay empty prompt.
// Must NOT call printf/malloc – only async-signal-safe functions.
static void sigint_interactive(int sig)
{
    g_sig = sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

// Heredoc: Ctrl+C → close readline's fd so read_full_input returns NULL.
static void sigint_heredoc(int sig)
{
    g_sig = sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    // Don't redisplay — heredoc will be cancelled by the NULL return
}

// Child processes inherit default disposition, so the kernel already handles
// SIGINT/SIGQUIT for them. The parent just needs to NOT die itself.
static void sigint_child(int sig)
{
    g_sig = sig;          // parent records it; child already dead/stopped
    write(STDOUT_FILENO, "\n", 1);
}

static void sigquit_child(int sig)
{
    g_sig = sig;
    write(STDERR_FILENO, "Quit: 3\n", 8);
}

// ─── public setup functions ───────────────────────────────────────────────────

// Call this in the main read-loop (interactive prompt, no child running).
void signals_interactive(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags   = SA_RESTART;   // restart rl_readline after signal
    sa_int.sa_handler = sigint_interactive;
    sigaction(SIGINT, &sa_int, NULL);

    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags   = 0;
    sa_quit.sa_handler = SIG_IGN;     // Ctrl+\ does nothing at the prompt
    sigaction(SIGQUIT, &sa_quit, NULL);
}

// Call this while waiting for a child process (after fork, before waitpid).
void signals_child(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags   = 0;
    sa_int.sa_handler = sigint_child;
    sigaction(SIGINT, &sa_int, NULL);

    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags   = 0;
    sa_quit.sa_handler = sigquit_child;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

// Call this inside the child process right after fork(), before execve().
// Restore default disposition so the child dies normally on Ctrl+C / Ctrl+\.
void signals_reset_child(void)
{
    signal(SIGINT,  SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

// Call this at the start of a heredoc read loop.
void signals_heredoc(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags   = 0;
    sa_int.sa_handler = sigint_heredoc;
    sigaction(SIGINT, &sa_int, NULL);

    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags   = 0;
    sa_quit.sa_handler = SIG_IGN;     // Ctrl+\ ignored in heredoc too
    sigaction(SIGQUIT, &sa_quit, NULL);
}
// Declared in signals.c

static char *build_prompt(t_env *env_list)
{
    char *cwd;
    char *prompt;

    cwd = NULL;
    if (!cwd)
        return (ft_strdup("minishell$ "));
    prompt = ft_strjoin(cwd, " minishell$ ");   // no "> " suffix
    return (prompt);
}

int main(int ac, char **av, char **env)
{
    char    *line;
    char    *prompt;
    t_lexer *tokens;
    t_env   *env_list;
    int     last_status;

    (void)ac;
    (void)av;
    last_status = 0;
    env_list = set_env(env);

    // Set interactive mode signals once before entering the loop
    signals_interactive();

    while (1)
    {
        g_sig = 0;                          // clear before every readline call
        prompt = build_prompt(env_list);
        line = read_full_input(prompt);     // readline blocks here
        free(prompt);

        // Ctrl+D → readline returns NULL → clean exit
        if (!line)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            break ;
        }

        // Ctrl+C during readline → g_sig == SIGINT, line is empty string
        if (g_sig == SIGINT)
        {
            free(line);
            last_status = 130;              // bash sets $? = 130 on SIGINT
            continue ;
        }

        if (*line == '\0')
        {
            free(line);
            continue ;
        }

        add_history(line);
        tokens = lex_line(line);
        free(line);
        if (!tokens)
            continue ;

        expand_lexer_tokens(tokens, last_status, env_list);

        // signals_child() / signals_reset_child() are called inside
        // are_we_gonna_split → your executor, around fork/waitpid
        last_status = are_we_gonna_split(tokens, &env_list);

        // After child finishes, re-arm interactive handlers
        signals_interactive();

        delete_lexer(&tokens);
    }
    return (last_status);
}