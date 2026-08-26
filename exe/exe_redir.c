/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:47:40 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:47:41 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_internal.h"

static int	open_redir_fd(t_redir *redir)
{
	int	fd;

	if (redir->type == TOKEN_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redir->file, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	return (fd);
}

static int	apply_heredoc(t_redir *redir)
{
	if (redir->fd < 0)
		return (0);
	dup2(redir->fd, STDIN_FILENO);
	close(redir->fd);
	redir->fd = -1;
	return (0);
}

int	apply_single_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == TOKEN_HEREDOC)
		return (apply_heredoc(redir));
	fd = open_redir_fd(redir);
	if (fd < 0)
		return (1);
	if (redir->type == TOKEN_REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	apply_redirections(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (apply_single_redir(redir))
			return (1);
		redir = redir->next;
	}
	return (0);
}
