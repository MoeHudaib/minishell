/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:25:12 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:25:53 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_internal.h"

static t_redir	*new_redir(t_token_type type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = ft_strdup(file);
	if (!redir->file)
		return (free(redir), NULL);
	redir->type = type;
	redir->fd = -1;
	redir->expand = 0;
	redir->next = NULL;
	return (redir);
}

void	add_redir(t_cmd *cmd, t_token_type type, char *file)
{
	t_redir	*new;
	t_redir	*current;

	new = new_redir(type, file);
	if (!new)
		return ;
	if (!cmd->redirs)
	{
		cmd->redirs = new;
		return ;
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = new;
}
