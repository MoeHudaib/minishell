#include "../organize.h"

static int	read_heredoc(t_redir *redir, int write_fd)
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

static int	copy_pipe(int from_fd, int to_fd)
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

static int	process_heredoc(t_redir *redir, int combined_pipe[2])
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

static int	fill_combined(t_cmd *cmd, int combined_pipe[2])
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

static int	assign_combined(t_cmd *cmd, int combined_pipe[2])
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

static int	multi_heredoc(t_cmd *cmd)
{
	int	combined_pipe[2];

	if (pipe(combined_pipe) < 0)
		return (0);
	if (!fill_combined(cmd, combined_pipe))
		return (0);
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