#include "../organize.h"

static char *handle_variable(const char *str, int *i, char **env)
{
    char    *var;
    char    *value;
    int     v = 0;

    var = malloc(ft_strlen(str) + 1);
    if (!var)
        return (NULL);
    while (is_valid_var_char(str[*i]))
        var[v++] = str[(*i)++];
    var[v] = '\0';
    value = get_env_value(var, env);
    free(var);
    return (value);
}

char *expand_dollar(const char *str, int *i, int last_status, char **env)
{
    char    *value;

    (*i)++;
    if (str[*i] == '?')
    {
        (*i)++;
        return (get_exit_status(last_status));
    }
    if (!str[*i] || !is_valid_var_char(str[*i]))
        return (ft_strdup("$"));
    value = handle_variable(str, i, env);
    return (value);
}

static char    *ft_strjoin_char(char *s, char c)
{
    char    buf[2];

    buf[0] = c;
    buf[1] = '\0';
    return (ft_strjoin(s, buf));
}

char *expand_token(char *token, int last_status, char **env)
{
    int     i = 0;
    char    quote = 0;
    char    *result;
    char    *value;
    char    *tmp;

    result = ft_strdup("");
    if (!result)
        return (NULL);
    while (token[i])
    {
        if (token[i] == '\'' && quote == 0)
        {
            quote = '\'';
            i++;
            continue;
        }
        if (token[i] == '\'' && quote == '\'')
        {
            quote = 0;
            i++;
            continue;
        }
        if (token[i] == '"' && quote == 0)
        {
            quote = '"';
            i++;
            continue;
        }
        if (token[i] == '"' && quote == '"')
        {
            quote = 0;
            i++;
            continue;
        }
        if (token[i] == '$' && quote != '\'')
        {
            value = expand_dollar(token, &i, last_status, env);
            tmp = ft_strjoin(result, value);
            free(result);
            free(value);
            result = tmp;
            continue;
        }
        tmp = ft_strjoin_char(result, token[i++]);
        free(result);
        result = tmp;
    }
    return (result);
}

void expand_lexer_tokens(t_lexer *tokens, int last_status, t_env *env)
{
    char **env_array = env_to_array(env);

    while (tokens)
    {
        if (!tokens->single_quote) // only expand if not single quoted
        {
            char *new = expand_token(tokens->token, last_status, env_array);
            free(tokens->token);
            tokens->token = new;
        }
        tokens = tokens->next;
    }
}