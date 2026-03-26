/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:48:57 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/26 15:24:35 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

int	has_unclosed_quotes(const char *s)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (s[i])
	{
		if (s[i] == '\\' && quote != '\'')
		{
			if (s[i + 1])
				i++;
		}
		else if (s[i] == '\'' || s[i] == '"')
		{
			if (!quote)
				quote = s[i];
			else if (quote == s[i])
				quote = 0;
		}
		i++;
	}
	return (quote != 0);
}

char	*read_full_input(char *str)
{
	char	*full;
	char	*temp;
	char	*joined;

	full = readline(str);
	while (full && has_unclosed_quotes(full))
	{
		temp = readline("> ");
		if (!temp)
		{
			free(full);
			return (NULL);
		}
		joined = malloc(ft_strlen(full) + ft_strlen(temp) + 2);
		sprintf(joined, "%s\n%s", full, temp);
		free(full);
		free(temp);
		full = joined;
	}
	return (full);
}

static t_lexer	*create_lexer_list(char **tokens, t_token_type *types)
{
	t_lexer	*head;
	t_lexer	*current;
	int		i;

	i = 0;
	head = lexer_init(tokens[i], types[i]);
	if (!head)
		return (NULL);
	i++;
	current = head;
	while (tokens[i])
	{
		current = add_new_token_back(&head, tokens[i], types[i]);
		if (!current)
			return (free_tokens(tokens, NULL, &head));
		i++;
	}
	return (head);
}

t_lexer	*lex_line(char *line)
{
	t_lexer			*lexer_head;
	char			**tokens;
	t_token_type	types[1024];

	if (!line)
		return (NULL);
	tokens = split_with_quotes(line, types);
	if (!tokens)
		return (NULL);
	lexer_head = create_lexer_list(tokens, types);
	free_tokens(tokens, NULL, NULL);
	return (lexer_head);
}

void	print_tokens(t_lexer *lexer_head)
{
	t_lexer	*current;

	if (!lexer_head)
	{
		return ;
	}
	current = lexer_head;
	while (current)
	{
		printf("%s", current->token);
		if (current->next)
			printf(" -> ");
		current = current->next;
	}
	printf("\n");
}
