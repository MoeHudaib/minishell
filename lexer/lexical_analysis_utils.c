/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:49:03 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:49:03 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

t_token_type	get_operator_type(const char *str, int i)
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

int	is_operator(const char *str, int i)
{
	return (str[i] == '|' || str[i] == '>'
		|| str[i] == '<' || str[i] == '&');
}

char	*extract_operator(const char *str, int *i)
{
	char	buf[3];
	int		b;

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

void	skip_spaces(const char *str, int *i)
{
	while (str[*i] == ' ')
		(*i)++;
}

int	handle_quote_char(const char *str, int *i, char *quote)
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
