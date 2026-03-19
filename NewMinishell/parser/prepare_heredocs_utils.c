/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_heredocs_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:12:50 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:12:50 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"

static int	count_heredocs(t_cmd *cmd)
{
	t_redir	*redir;
	int		count;

	count = 0;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
			count++;
		redir = redir->next;
	}
	return (count);
}

static int	single_heredoc(t_cmd *cmd)
{
	t_redir	*redir;
	int		pipe_fd[2];

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			if (pipe(pipe_fd) < 0)
				return (0);
			if (!read_heredoc(redir, pipe_fd[1]))
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				return (0);
			}
			close(pipe_fd[1]);
			redir->fd = pipe_fd[0];
		}
		redir = redir->next;
	}
	return (1);
}

static int	multi_heredoc(t_cmd *cmd)
{
	int	combined_pipe[2];

	if (pipe(combined_pipe) < 0)
		return (0);
	if (!fill_combined(cmd, combined_pipe))
	{
		close(combined_pipe[0]);
		return (0);
	}
	return (assign_combined(cmd, combined_pipe));
}

static int	prepare_cmd_heredocs(t_cmd *cmd)
{
	if (count_heredocs(cmd) <= 1)
		return (single_heredoc(cmd));
	return (multi_heredoc(cmd));
}

int	prepare_heredocs(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (!prepare_cmd_heredocs(current))
			return (0);
		current = current->next;
	}
	return (1);
}
