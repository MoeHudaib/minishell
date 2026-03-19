/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:48:33 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:48:33 by mohammad         ###   ########.fr       */
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

void	handle_single_quote(int *i, char *quote)
{
	if (*quote == 0)
		*quote = '\'';
	else if (*quote == '\'')
		*quote = 0;
	(*i)++;
}

void	handle_double_quote(int *i, char *quote)
{
	if (*quote == 0)
		*quote = '"';
	else if (*quote == '"')
		*quote = 0;
	(*i)++;
}

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}
