/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:22:44 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:25:57 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_internal.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static void	copy_and_free(char **dst, char **src, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	free(src);
}

void	add_arg(t_cmd *cmd, char *arg)
{
	char	**new_args;
	char	*duped;
	int		len;

	len = 0;
	if (cmd->args)
		while (cmd->args[len])
			len++;
	new_args = malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return ;
	duped = ft_strdup(arg);
	if (!duped)
		return (free(new_args));
	if (cmd->args)
		copy_and_free(new_args, cmd->args, len);
	new_args[len] = duped;
	new_args[len + 1] = NULL;
	cmd->args = new_args;
}

void	append_cmd(t_cmd **head, t_cmd *cmd)
{
	t_cmd	*current;

	if (!*head)
	{
		*head = cmd;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = cmd;
}

int	is_redir(t_token_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND
		|| type == TOKEN_HEREDOC);
}
