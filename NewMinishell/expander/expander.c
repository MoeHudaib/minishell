/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:11:42 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/26 16:47:45 by mhdeeb           ###   ########.fr       */
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

char	*append_char(char *result, char c)
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
	int	old_i;

	i = ctx->i;
	while (token[*i] && result)
	{
		if (token[*i] == '\\')
			result = handle_backslash(result, token, i, *quote);
		else if (token[*i] == '\'' || token[*i] == '"')
			result = handle_quote(result, token, i, quote);
		else if (token[*i] == '$' && *quote != '\'')
		{
			old_i = *i;
			result = append_dollar(result, ctx);
			if (*i == old_i)
				(*i)++;
		}
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
		if (!tokens->single_quote && tokens->token)
		{
			expanded = expand_token(tokens->token, last_status, env_array);
			if (expanded)
			{
				free(tokens->token);
				tokens->token = expanded;
			}
		}
		tokens = tokens->next;
	}
	free_2d_array(env_array);
}

// int main(int ac, char **av, char **env)
// {
// 	t_lexer	*head;

// 	char *line = readline(">");
// 	head = lex_line(line);
// 	print_tokens(head);
// 	printf("\n\n");
// 	t_env *envlist = set_env(env);
// 	expand_lexer_tokens(head, 0, envlist);
// 	print_tokens(head);
// }
