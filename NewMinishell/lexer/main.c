#include <stdio.h>
#include "../exe/exe.h"

char **organize(t_lexer *tokens)
{
    char **cmd;
    int count = 0;
    t_lexer *tmp = tokens;

    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    cmd = malloc(sizeof(char *) * (count + 1));

    int i = 0;
    while (tokens)
    {
        cmd[i++] = ft_strdup(tokens->token);
        tokens = tokens->next;
    }

    cmd[i] = NULL;

    return cmd;
}
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

int main(int ac, char **av, char **env)
{
    char *line;
    t_lexer *tokens;
    char **cmd;
    t_env *env_list;
    int last_status = 0;

    env_list = set_env(env);

    while (1)
    {
        line = read_full_input("minishell> ");

        if (!line)
            continue;

        add_history(line);

        tokens = lex_line(line);

        if (!tokens)
        {
            free(line);
            continue;
        }

        expand_lexer_tokens(tokens, last_status, env_list);
        // cmd = check(tokens);
        cmd = organize(tokens);

        last_status = execute_command(cmd, &env_list);

        free_tokens(cmd, NULL, NULL);
        delete_lexer(&tokens);
        free(line);
    }
}
/*
For some reason the line cannot be freed inside the functions, 
it only worked when i freed it inside the main!

must discover the reason in order to understand the proplem correctly.
*/

// ls -> -ls -> -m 
 