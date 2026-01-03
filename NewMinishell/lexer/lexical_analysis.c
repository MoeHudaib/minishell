#include "lexer.h"

static void skip_spaces(const char *str, int *i)
{
    while (str[*i] == ' ')
        (*i)++;
}

static int handle_quote_char(const char *str, int *i, char *quote)
{
    if (!*quote && (str[*i] == '"' || str[*i] == '\''))
    {
        *quote = str[*i];
        (*i)++;
        return (1);
    }
    else if (*quote && str[*i] == *quote)
    {
        *quote = 0;
        (*i)++;
        return (1);
    }
    return (0);
}

static void process_char(const char *str, int *i, char *buffer, int *b, char quote)
{
    if (quote == '"' && str[*i] == '\\' && str[*i + 1])
    {
        if (str[*i + 1] == '"' || str[*i + 1] == '\\' ||
            str[*i + 1] == '$' || str[*i + 1] == '`')
        {
            buffer[(*b)++] = str[*i + 1];
            *i += 2;
            return;
        }
    }
    if (!quote && str[*i] == '\\' && str[*i + 1])
    {
        buffer[(*b)++] = str[*i + 1];
        *i += 2;
        return;
    }
    buffer[(*b)++] = str[*i];
    (*i)++;
}

static char *parse_word(const char *str, int *i)
{
    char buffer[1024];
    int b = 0;
    char quote = 0;

    while (str[*i])
    {
        if (handle_quote_char(str, i, &quote))
            continue;
        if (!quote && str[*i] == ' ')
            break;
        process_char(str, i, buffer, &b, quote);
    }
    buffer[b] = '\0';
    return ft_strdup(buffer);
}

char **split_with_quotes(const char *str)
{
    char **result = malloc(1024 * sizeof(char *));
    int count = 0;
    int i = 0;

    while (str[i])
    {
        skip_spaces(str, &i);
        if (!str[i])
            break;
        result[count++] = parse_word(str, &i);
    }
    result[count] = NULL;
    return result;
}
