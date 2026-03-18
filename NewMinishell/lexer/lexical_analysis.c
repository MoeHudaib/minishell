#include "../organize.h"

static t_token_type	get_operator_type(const char *str, int i)
{
	if (str[i] == '|' && str[i + 1] == '|')
		return (TOKEN_OR);
	if (str[i] == '&' && str[i + 1] == '&')
		return (TOKEN_AND);
	if (str[i] == '>' && str[i + 1] == '>')
		return (TOKEN_REDIR_APPEND);
	if (str[i] == '<' && str[i + 1] == '<')
		return (TOKEN_HEREDOC);
	if (str[i] == '|')
		return (TOKEN_PIPE);
	if (str[i] == '>')
		return (TOKEN_REDIR_OUT);
	if (str[i] == '<')
		return (TOKEN_REDIR_IN);
	return (TOKEN_WORD);
}

static int	is_operator(const char *str, int i)
{
	return (str[i] == '|' || str[i] == '>' ||
			str[i] == '<' || str[i] == '&');
}

static char	*extract_operator(const char *str, int *i)
{
	char	buf[3];
	int		b;

	// handle double operators: ||, >>. <<, &&
	b = 0;
	if ((str[*i] == '|' && str[*i + 1] == '|') 
		|| (str[*i] == '>' && str[*i + 1] == '>')
		|| (str[*i] == '<' && str[*i + 1] == '<')
		|| (str[*i] == '&' && str[*i + 1] == '&'))
	{
		buf[b++] = str[(*i)++];
		buf[b++] = str[(*i)++];
	}
	else
		buf[b++] = str[(*i)++];
	buf[b] = '\0';
	return (ft_strdup(buf));
}

static void	skip_spaces(const char *str, int *i)
{
	while (str[*i] == ' ')
		(*i)++;
}

static int	handle_quote_char(const char *str, int *i, char *quote)
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

static void	process_char(const char *str, int *i, char *buffer, int *b, char quote)
{
	if (quote == '"' && str[*i] == '\\' && str[*i + 1])
	{
		if (str[*i + 1] == '"' || str[*i + 1] == '\\'
            || str[*i + 1] == '$' || str[*i + 1] == '`')
		{
			buffer[(*b)++] = str[*i + 1];
			*i += 2;
			return ;
		}
	}
	if (!quote && str[*i] == '\\' && str[*i + 1])
	{
		buffer[(*b)++] = str[*i + 1];
		*i += 2;
		return ;
	}
	buffer[(*b)++] = str[*i];
	(*i)++;
}

static char	*parse_word(const char *str, int *i)
{
	char	buffer[1024];
	int		b;
	char	quote;

	quote = 0;
	b = 0;
	while (str[*i])
	{
		if (handle_quote_char(str, i, &quote))
		{
			buffer[b++] = str[*i - 1]; // keep the quote char
			continue ;
		}
		if (!quote && str[*i] == ' ')
			break ;
		if (!quote && is_operator(str, *i))
			break ;
		buffer[b++] = str[(*i)++];
	}
	buffer[b] = '\0';
	return (ft_strdup(buffer));
}

static void	handle_operator(const char *str, int *i,
				char **result, t_token_type *types, int *count)
{
	types[*count] = get_operator_type(str, *i);
	result[*count] = extract_operator(str, i);
	(*count)++;
}

static void	handle_word(const char *str, int *i,
				char **result, t_token_type *types, int *count)
{
	types[*count] = TOKEN_WORD;
	result[*count] = parse_word(str, i);
	(*count)++;
}

char	**split_with_quotes(const char *str, t_token_type *types)
{
	char	**result;
	int		count;
	int		i;

	result = malloc(1024 * sizeof(char *));
	if (!result)
		return (NULL);
	count = 0;
	i = 0;
	while (str[i])
	{
		skip_spaces(str, &i);
		if (!str[i])
			break ;
		if (is_operator(str, i))
			handle_operator(str, &i, result, types, &count);
		else
			handle_word(str, &i, result, types, &count);
	}
	result[count] = NULL;
	return (result);
}
