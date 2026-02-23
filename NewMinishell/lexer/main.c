#include <stdio.h>
#include "../builtins/builtins.h"

void    organize(t_lexer *tokens, char **env)
{
    t_lexer *head;
    char *path;

    if (!tokens)
        return ;
    head = tokens;
    while (head)
    {
        path = build_path(env, head->token);
        if (path)
        {
            if (!is_builtin(head->token))
                break ;
            printf("%s -> valid\n", path);
        }
        else
        {
            if (!is_builtin(head->token))
                break ;
            printf("%s -> not a valid command\n", head->token);
        }
        head = head->next;
    }
} // i don't understand the results when running (pwd pwds) and (pwds pwd)


int main(int ac, char **av, char **env)
{
    char    pwd[1024];
    getcwd(pwd, 1024);
    char    *line;
    t_lexer *tokens;
    while (1)
    {
        line = read_full_input(pwd);
        if (!line)
            continue ;
        tokens = lex_line(line);
        if (!tokens)
            return (1);
        add_history(line);
        organize(tokens, env); // Tokens are done here, the next step is to parse them (Organize them in order to be executed)
                                // Tokens are organized and now we know if they are a valid commands or not
                                // The next step is to build the built ins functions then determen if the none valid commands are of them or just not valid fr
                                // then execute them properly
        delete_lexer(&tokens);
        free(line);
    }
}
/*
For some reason the line cannot be freed inside the functions, 
it only worked when i freed it inside the main!

must discover the reason in order to understand the proplem correctly.
*/