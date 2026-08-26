/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_heredocs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:59:06 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:59:07 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"

int	read_heredoc(t_redir *redir, int write_fd)
{
	char	*line;

	while (1)
	{
		signals_heredoc();
		line = readline("> ");
		if (!line)
			break ;
		if (g_sig == SIGINT)
		{
			free(line);
			return (0);
		}
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	return (1);
}

int	copy_pipe(int from_fd, int to_fd)
{
	char	buf[4096];
	ssize_t	n;

	n = read(from_fd, buf, sizeof(buf));
	while (n > 0)
	{
		write(to_fd, buf, n);
		n = read(from_fd, buf, sizeof(buf));
	}
	return (1);
}

int	process_heredoc(t_redir *redir, int combined_pipe[2])
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) < 0)
		return (close(combined_pipe[0]), close(combined_pipe[1]), 0);
	if (!read_heredoc(redir, pipe_fd[1]))
		return (close(pipe_fd[0]), close(pipe_fd[1]),
			close(combined_pipe[0]), close(combined_pipe[1]), 0);
	close(pipe_fd[1]);
	copy_pipe(pipe_fd[0], combined_pipe[1]);
	close(pipe_fd[0]);
	redir->fd = -1;
	return (1);
}

int	fill_combined(t_cmd *cmd, int combined_pipe[2])
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
			if (!process_heredoc(redir, combined_pipe))
				return (0);
		redir = redir->next;
	}
	return (1);
}

int	assign_combined(t_cmd *cmd, int combined_pipe[2])
{
	t_redir	*redir;

	close(combined_pipe[1]);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			redir->fd = combined_pipe[0];
			return (1);
		}
		redir = redir->next;
	}
	close(combined_pipe[0]);
	return (1);
}
