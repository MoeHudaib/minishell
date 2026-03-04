#include <stdio.h>
#include "../exe/exe.h"

char    **organize(t_lexer *tokens, char **env)
{
    t_lexer *head;
    char *path;
    char **cmd;
    int i = 0;

    if (!tokens)
        return NULL;
    head = tokens;
    cmd = malloc(sizeof(char *) * 1024);
    while (head)
    {
        path = build_path(env, head->token);
        if (path)
        {
            if (!is_builtin(head->token))
                break ;
        }
        else
        {
            if (!is_builtin(head->token))
                break ;
        }
        cmd[i++] = head->token;
        head = head->next;
    }
    cmd[i] = NULL;
    return (cmd);
} // i don't understand the results when running (pwd pwds) and (pwds pwd)


int main(int ac, char **av, char **env)
{
    char    pwd[1024];
    getcwd(pwd, 1024);
    char    *line;
    char **cmd;
    t_lexer *tokens;
    while (1)
    {
        line = read_full_input(pwd);
        if (!line)
            continue ;
        tokens = lex_line(line);
        if (!tokens)
        {
            free(line);
            continue ;
        }
        add_history(line);
        cmd = organize(tokens, env); // Tokens are done here, the next step is to parse them (Organize them in order to be executed)
                                // Tokens are organized and now we know if they are a valid commands or not
                                // The next step is to build the built ins functions then determen if the none valid commands are of them or just not valid fr
                                // then execute them properly
        if (is_it_simple_exe(line) == YES)
            simple_exe(ac, cmd, env);
        delete_lexer(&tokens);
        free(line);
    }
}
/*
For some reason the line cannot be freed inside the functions, 
it only worked when i freed it inside the main!

must discover the reason in order to understand the proplem correctly.
*/