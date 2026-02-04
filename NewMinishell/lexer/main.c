#include <stdio.h>
#include "lexer.h"

void    organize(t_lexer *tokens)
{
    t_lexer *head;
    char    **env;
    char *path;

    if (!tokens)
        return ;
    head = tokens;
    while (head)
    {
        path = build_path(env, head->token);
        if (path)
            printf("\n%s", path);
        head = head->next;
    }
}


int main(void)
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
        print_tokens(tokens); // Tokens are done here, the next step is to parse them (Organize them in order to be executed)

        delete_lexer(&tokens);
        free(line);
    }
}
/*
For some reason the line cannot be freed inside the functions, 
it only worked when i freed it inside the main!

must discover the reason in order to understand the proplem correctly.
*/