#include "../organize.h"

int has_complex(t_lexer *tokens)
{
    while (tokens)
    {
        if (strncmp(tokens->token, ">", 2) == 0 && tokens->next)
        {
            // output redirection — handled separately, skip for now
            tokens = tokens->next;
        }
        else if (strncmp(tokens->token, ">>", 3) == 0 && tokens->next)
        {
            // append redirection — handled separately, skip for now
            tokens = tokens->next;
        }
        else if (strncmp(tokens->token, "<", 2) == 0 && tokens->next)
        {
            // input redirection — handled separately, skip for now
            tokens = tokens->next;
        }
        else if (strncmp(tokens->token, "|", 2) == 0)
        {
            // pipe — handled separately, skip for now
        }
        else if (strncmp(tokens->token, "&&", 3) == 0)
        {
            // AND — handled separately, skip for now
        }
        else if (strncmp(tokens->token, "||", 3) == 0)
        {
            // OR — handled separately, skip for now
        }
        else if (strncmp(tokens->token, "<<", 3) == 0 && tokens->next)
        {
            // heredoc — handled separately, skip for now
            tokens = tokens->next;
        }
        else
        {
            tokens = tokens->next;
            continue ;
        }
        return (YES); // if we found any of the above, it's complex
    }
    return (NO); // no complex tokens found, it's simple
}
