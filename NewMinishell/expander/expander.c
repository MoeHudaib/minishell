#include "../organize.h"

static char	*handle_variable(const char *str, int *i, char **env)
{
	char	*var;
	char	*value;
	int		v;

	v = 0;
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

char	*expand_dollar(const char *str, int *i, int last_status, char **env)
{
	char	*value;

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

static char	*ft_strjoin_char(char *s, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (ft_strjoin(s, buf));
}

static void	handle_single_quote(int *i, char *quote)
{
	if (*quote == 0)
		*quote = '\'';
	else if (*quote == '\'')
		*quote = 0;
	(*i)++;
}

static void	handle_double_quote(int *i, char *quote)
{
	if (*quote == 0)
		*quote = '"';
	else if (*quote == '"')
		*quote = 0;
	(*i)++;
}

static char	*append_dollar(char *result, char *token,
				int *i, int last_status, char **env)
{
	char	*value;
	char	*tmp;

	value = expand_dollar(token, i, last_status, env);
	tmp = ft_strjoin(result, value);
	free(result);
	free(value);
	return (tmp);
}

static char	*append_char(char *result, char c)
{
	char	*tmp;

	tmp = ft_strjoin_char(result, c);
	free(result);
	return (tmp);
}

char	*expand_token(char *token, int last_status, char **env)
{
	char	*result;
	char	quote;
	int		i;

	i = 0;
	quote = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (token[i])
	{
		if (token[i] == '\'')
			handle_single_quote(&i, &quote);
		else if (token[i] == '"')
			handle_double_quote(&i, &quote);
		else if (token[i] == '$' && quote != '\'')
			result = append_dollar(result, token, &i, last_status, env);
		else
			result = append_char(result, token[i++]);
	}
	return (result);
}

void	expand_lexer_tokens(t_lexer *tokens, int last_status, t_env *env)
{
	char	**env_array;
	char	*expanded;

	env_array = env_to_array(env);
	while (tokens)
	{
		if (!tokens->single_quote)
		{
			expanded = expand_token(tokens->token, last_status, env_array);
			free(tokens->token);
			tokens->token = expanded;
		}
		tokens = tokens->next;
	}
	free(env_array);
}