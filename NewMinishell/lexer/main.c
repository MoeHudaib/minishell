#include "../organize.h"

// void organize01(t_lexer *tokens, t_env **head)// If the line to be executed has built-in
// {
//     t_lexer *current;
//     char *line;

//     if (!tokens)
//         return;

//     current = tokens;
//     line = strdup("");

//     if (strncmp(current->token, "export", 6) == 0)
//     {
//         current = current->next;
//         while (current)
//         {
//             char *tmp = line;
//             line = ft_strjoin(tmp, current->token);
//             free(tmp);
//             current = current->next;
//         }
//         ft_export(head, line);
//     }
//     else if (strncmp(current->token, "unset", 5) == 0)
//     {
//         current = current->next;
//         if (current)
//             ft_unset(head, current->token);
//     }

//     free(line);
// }

void    prnt_tokens(t_lexer *tokens)
{
    t_lexer *current;

    current = tokens;
    while (current)
    {
        printf("%s\n", current->token);
        current = current->next;
    }
}

int g_signal = 0;

static void handle_sigint(int sig)
{
    (void)sig;
    g_signal = 130;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

static void setup_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

static char *build_prompt(t_env *env_list)
{
    char *cwd;
    char *prompt;

    cwd = get_env_value(env_list, "PWD");  // or use getcwd()
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
    setup_signals();
    while (1)
    {
        prompt = build_prompt(env_list);
        line = read_full_input(prompt);
        free(prompt);
        if (!line)          // ctrl+D
        {
            write(1, "exit\n", 5);
            break ;
        }
        if (g_signal)       // ctrl+C was pressed during input
        {
            last_status = g_signal;
            g_signal = 0;
            free(line);
            continue ;
        }
        if (line[0] == '\0') // empty line
        {
            free(line);
            continue ;
        }
        add_history(line);
        tokens = lex_line(line);
        if (!tokens)
        {
            free(line);
            continue ;
        }
        expand_lexer_tokens(tokens, last_status, env_list);
        if (has_complex(tokens))
            last_status = are_we_gonna_split(tokens, &env_list);
        else
        {
            char **cmd = organize(tokens);
            if (cmd && cmd[0])
                execute_command(cmd, &env_list);
            free_tokens(cmd, NULL, NULL);
        }
        delete_lexer(&tokens);
        free(line);
    }
    return (last_status);
}
/*
For some reason the line cannot be freed inside the functions, 
it only worked when i freed it inside the main!

must discover the reason in order to understand the proplem correctly.
*/

// ls -> -ls -> -m 
 