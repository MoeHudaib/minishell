#include "lexer.h"

static  int has_complex(char c1, char c2)
{
    if (c1 == '>' && c2 == '>')
        return (YES);
    else if (c1 == '<' && c2 == '<')
        return (YES);
    else if (c1 == '<')
        return (YES);
    else if (c1 == '>')
        return (YES);
    return (NO);
}

int is_it_simple_exe(char   *line)
{
    int i;

    i = 0;
    while (line[i] && line[i + 1])
    {
        if (line[i] == '|' || has_complex(line[i], line[i+1]) == YES)
            return (NO);
        i++;
    }
    return (YES);
}