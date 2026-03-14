#include "../organize.h"

void    prnt_tokens(t_lexer *tokens)
{
    t_lexer *current;

    current = tokens;
    while (current)
    {
        printf("%s tpye(%i) single(%i) double(%i)\n", current->token, current->type, current->single_quote, current->double_quote);
        current = current->next;
    }
}

// int g_signal = 0;

// static void handle_sigint(int sig)
// {
//     (void)sig;
//     g_signal = 130;
//     write(1, "\n", 1);
//     rl_on_new_line();
//     rl_replace_line("", 0);
//     rl_redisplay();
// }

// static void setup_signals(void)
// {
//     struct sigaction sa_int;
//     struct sigaction sa_quit;

//     sa_int.sa_handler = handle_sigint;
//     sigemptyset(&sa_int.sa_mask);
//     sa_int.sa_flags = SA_RESTART;
//     sigaction(SIGINT, &sa_int, NULL);
//     sa_quit.sa_handler = SIG_IGN;
//     sigemptyset(&sa_quit.sa_mask);
//     sa_quit.sa_flags = 0;
//     sigaction(SIGQUIT, &sa_quit, NULL);
// }

static char *build_prompt(t_env *env_list)
{
    char *cwd;
    char *prompt;

    cwd = get_env_value("PWD", env_to_array(env_list));  // or use getcwd()
    if (!cwd)
        return (ft_strdup("minishell> "));
    prompt = ft_strjoin(cwd, " minishell> ");
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
    // setup_signals();
    while (1)
    {
        prompt = build_prompt(env_list);
        line = read_full_input(prompt);
        free(prompt);
        if (!line)
            break ;
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
        last_status = are_we_gonna_split(tokens, &env_list);
        delete_lexer(&tokens);
    }
    return (last_status);
}
// lex_line -> split_with_quotes -> read_full_input -> has_unclosed_quotes