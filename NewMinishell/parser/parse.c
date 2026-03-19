/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:25:19 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:25:19 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_internal.h"

static t_cmd	*syntax_error(t_cmd *head, t_cmd *current, char *token)
{
	if (token)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		ft_putstr_fd(token, STDERR_FILENO);
		ft_putendl_fd("'", STDERR_FILENO);
	}
	else
		ft_putendl_fd(
			"minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
	current->next = NULL;
	free_cmd_list(head);
	free_cmd_list(current);
	return (NULL);
}

static t_cmd	*handle_redir(t_lexer **tokens, t_cmd *head, t_cmd *current)
{
	t_token_type	redir_type;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens)
		return (syntax_error(head, current, NULL));
	if ((*tokens)->type != TOKEN_WORD)
		return (syntax_error(head, current, (*tokens)->token));
	add_redir(current, redir_type, (*tokens)->token);
	return (current);
}

static t_cmd	*parse_token(t_lexer **tok, t_cmd **head, t_cmd *current)
{
	t_cmd	*next_cmd;

	if ((*tok)->type == TOKEN_WORD)
		add_arg(current, (*tok)->token);
	else if ((*tok)->type == TOKEN_PIPE)
	{
		append_cmd(head, current);
		next_cmd = new_cmd();
		if (!next_cmd)
			return (free_cmd_list(*head), NULL);
		return (next_cmd);
	}
	else if (is_redir((*tok)->type))
	{
		if (!handle_redir(tok, *head, current))
			return (NULL);
	}
	return (current);
}

t_cmd	*parse(t_lexer *tokens)
{
	t_cmd	*head;
	t_cmd	*current;

	head = NULL;
	current = new_cmd();
	if (!current)
		return (NULL);
	while (tokens)
	{
		current = parse_token(&tokens, &head, current);
		if (!current)
			return (NULL);
		tokens = tokens->next;
	}
	append_cmd(&head, current);
	return (head);
}
