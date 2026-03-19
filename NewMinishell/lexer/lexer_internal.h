/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:48:54 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:48:54 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "../organize.h"

/*
** Internal helpers shared between lexical_analysis.c
** and lexical_analysis_utils.c ONLY.
** Do NOT include this anywhere else.
*/

typedef struct s_pchar
{
	int		*i;
	char	*buf;
	int		*b;
	char	q;
}	t_pchar;

t_token_type	get_operator_type(const char *str, int i);
int				is_operator(const char *str, int i);
char			*extract_operator(const char *str, int *i);
void			skip_spaces(const char *str, int *i);
int				handle_quote_char(const char *str, int *i, char *quote);

#endif