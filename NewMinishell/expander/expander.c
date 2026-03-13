#include "../organize.h"

static char *handle_variable(const char *str, int *i, char **env)
{
    char var[256];
    int v = 0;

    while (is_valid_var_char(str[*i]))
        var[v++] = str[(*i)++];
    var[v] = '\0';
    return (get_env_value(var, env));
}

char *expand_dollar(const char *str, int *i, char quote, int last_status, char **env)
{
    char *value;
    if (quote == '\'')
        return (ft_strdup("$"));
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

char *expand_token(char *token, int last_status, char **env)
{
    int i = 0;
    char quote = 0;
    char result[2048];
    int r = 0;

    while (token[i])
    {
        if ((token[i] == '\'' || token[i] == '"'))
        {
            if (!quote)
                quote = token[i];
            else if (quote == token[i])
                quote = 0;
            result[r++] = token[i++];
            continue;
        }

        if (token[i] == '$')
        {
            char *value = expand_dollar(token, &i, quote, last_status, env);
            int j = 0;
            while (value[j])
                result[r++] = value[j++];
            free(value);
            continue;
        }

        result[r++] = token[i++];
    }

    result[r] = '\0';
    return (ft_strdup(result));
}

void expand_lexer_tokens(t_lexer *tokens, int last_status, t_env *env)
{
    char **env_array = env_to_array(env);

    while (tokens)
    {
        char *new = expand_token(tokens->token, last_status, env_array);
        free(tokens->token);
        tokens->token = new;
        tokens = tokens->next;
    }

    // free env array
}