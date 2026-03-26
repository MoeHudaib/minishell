/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:48:33 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/26 16:37:52 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

char	*ft_strjoin_char(char *s, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (ft_strjoin(s, buf));
}

char	*handle_quote(char *result, char *token, int *i, char *quote)
{
	char	c;

	c = token[*i];
	if (*quote == 0)
	{
		*quote = c;
		(*i)++;
		return (result);
	}
	else if (*quote == c)
	{
		*quote = 0;
		(*i)++;
		return (result);
	}
	else
		return (append_char(result, token[(*i)++]));
}

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*handle_backslash(char *result, char *token, int *i, char quote)
{
	char	next;

	next = token[*i + 1];
	if (quote == '\'')
		return (append_char(result, token[(*i)++]));
	if (next)
	{
		(*i)++;
		result = append_char(result, token[*i]);
		(*i)++;
		return (result);
	}
	return (append_char(result, token[(*i)++]));
}
