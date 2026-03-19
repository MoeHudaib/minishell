/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:11:42 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:13:38 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

static char	*append_dollar(char *result, t_expand *ctx)
{
	char	*value;
	char	*tmp;

	value = expand_dollar(ctx->token, ctx->i, ctx->last_status, ctx->env);
	if (!value)
		return (free(result), NULL);
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

static char	*expand_loop(char *result, char *token,
	t_expand *ctx, char *quote)
{
	int	*i;

	i = ctx->i;
	while (token[*i] && result)
	{
		if (token[*i] == '\'')
			handle_single_quote(i, quote);
		else if (token[*i] == '"')
			handle_double_quote(i, quote);
		else if (token[*i] == '$' && *quote != '\'')
			result = append_dollar(result, ctx);
		else
			result = append_char(result, token[(*i)++]);
		ctx->q = *quote;
	}
	return (result);
}

char	*expand_token(char *token, int last_status, char **env)
{
	char		*result;
	char		quote;
	int			i;
	t_expand	ctx;

	i = 0;
	quote = 0;
	ctx = (t_expand){token, &i, last_status, env, 0};
	result = ft_strdup("");
	if (!result)
		return (NULL);
	result = expand_loop(result, token, &ctx, &quote);
	return (result);
}

void	expand_lexer_tokens(t_lexer *tokens, int last_status, t_env *env)
{
	char	**env_array;
	char	*expanded;

	env_array = env_to_array(env);
	if (!env_array)
		return ;
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
