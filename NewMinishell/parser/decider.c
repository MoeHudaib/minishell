#include "parse.h"

int has_complex(t_lexer *tokens)
{
    if (!tokens)
        return(1);

    if (strncmp(tokens->token, ">", 2) == 0)
        return (YES);
    else if (strncmp(tokens->token, ">>", 3) == 0)
        return (YES);
    else if (strncmp(tokens->token, "<", 2) == 0)
        return (YES);
    else if (strncmp(tokens->token, "<<", 3) == 0)
        return (YES);
    else if (strncmp(tokens->token, "|", 2) == 0)
        return (YES);
    return (NO);
}

// EXAMPLE: cat file | grep "something" | ls -l
//   child process          child process         child process
//    [cat file]   [pipe]  [grep something] [pipe] [ls -l]
//    head->tokens pipe  head->tokens  pipe head->tokens 

int split_into_processes(t_lexer *head) // The goal here is to produce a char **tokens inside the lexer sturct in order to execute it directly 
{
    // here we are going to split the execution
}

int are_we_gonna_split(t_lexer *head)
{
    t_lexer *current;
    int     len_of_exe;

    if (!head)
        return (1);
    current = head;
    len_of_exe = 0;
    while (current)
    {
        if (has_complex(current->token) == YES)
        {
            //split_into_processes(head);
            len_of_exe++;
            return (1);
        }
        current = current->next;
    }
    return (len_of_exe);
}