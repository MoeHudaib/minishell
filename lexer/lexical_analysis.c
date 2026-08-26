/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:49:05 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/26 16:45:49 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

static void	process_escape(const char *str, int *i, char *buf, int *b)
{
	if (str[*i + 1])
	{
		buf[(*b)++] = '\\';
		buf[(*b)++] = str[*i + 1];
		*i += 2;
	}
}

static void	process_char(const char *str, t_pchar *pc)
{
	if (pc->q == '"' && str[*pc->i] == '\\' && str[*pc->i + 1])
	{
		process_escape(str, pc->i, pc->buf, pc->b);
		return ;
	}
	if (!pc->q && str[*pc->i] == '\\' && str[*pc->i + 1])
	{
		pc->buf[(*pc->b)++] = '\\';
		pc->buf[(*pc->b)++] = str[*pc->i + 1];
		*pc->i += 2;
		return ;
	}
	pc->buf[(*pc->b)++] = str[*pc->i];
	(*pc->i)++;
}

static char	*parse_word(const char *str, int *i)
{
	char	buffer[1024];
	int		b;
	char	quote;
	t_pchar	pc;

	quote = 0;
	b = 0;
	pc = (t_pchar){i, buffer, &b, 0};
	while (str[*i])
	{
		if (handle_quote_char(str, i, &quote))
		{
			buffer[b++] = str[*i - 1];
			continue ;
		}
		if (!quote && str[*i] == ' ')
			break ;
		if (!quote && is_operator(str, *i))
			break ;
		pc.q = quote;
		process_char(str, &pc);
	}
	buffer[b] = '\0';
	return (ft_strdup(buffer));
}

static void	handle_token(const char *str, int *i, t_split_ctx *ctx)
{
	if (is_operator(str, *i))
	{
		ctx->types[ctx->count] = get_operator_type(str, *i);
		ctx->result[ctx->count] = extract_operator(str, i);
	}
	else
	{
		ctx->types[ctx->count] = TOKEN_WORD;
		ctx->result[ctx->count] = parse_word(str, i);
	}
	ctx->count++;
}

char	**split_with_quotes(const char *str, t_token_type *types)
{
	t_split_ctx	ctx;
	int			i;

	ctx.result = malloc(1024 * sizeof(char *));
	if (!ctx.result)
		return (NULL);
	ctx.types = types;
	ctx.count = 0;
	i = 0;
	while (str[i])
	{
		skip_spaces(str, &i);
		if (!str[i])
			break ;
		handle_token(str, &i, &ctx);
	}
	ctx.result[ctx.count] = NULL;
	return (ctx.result);
}
