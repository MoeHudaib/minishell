#include "../organize.h"

static void	close_pipes(int len, int fd[][2])
{
	int	i;

	i = 0;
	while (i < len)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

static void	setup_pipe_fds(int i, int len_of_exe, int fd[][2])
{
	if (i == 0)
		dup2(fd[i][1], STDOUT_FILENO);
	else if (i == len_of_exe - 1)
		dup2(fd[i - 1][0], STDIN_FILENO);
	else
	{
		dup2(fd[i - 1][0], STDIN_FILENO);
		dup2(fd[i][1], STDOUT_FILENO);
	}
}

static int	fork_child(int i, int len_of_exe, int fd[][2],
				t_cmd *current, t_env **env, int last_status)
{
	int	j;

	j = 0;
	setup_pipe_fds(i, len_of_exe, fd);
	while (j < len_of_exe - 1)
	{
		close(fd[j][0]);
		close(fd[j][1]);
		j++;
	}
	if (apply_redirections(current))
		exit(1);
	execute_command(current->args, env, last_status);
	return (0);
}

int	create_processes(int len_of_exe, int fd[][2],
		t_cmd *cmd_list, t_env **env, pid_t *pids, int last_status)
{
	int		i;
	t_cmd	*current;

	i = 0;
	current = cmd_list;
	while (i < len_of_exe)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			return (1);
		if (pids[i] == 0)
		{
			signals_reset_child();
			fork_child(i, len_of_exe, fd, current, env, last_status);
		}
		current = current->next;
		i++;
	}
	return (0);
}

static int	collect_status(int len_of_exe, pid_t *pids)
{
	int	i;
	int	status;
	int	last_status1;

	i = 0;
	last_status1 = 0;
	while (i < len_of_exe)
	{
		waitpid(pids[i], &status, 0);
		if (i == len_of_exe - 1)
		{
			if (WIFSIGNALED(status))
			{
				last_status1 = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGQUIT)
					write(STDERR_FILENO, "Quit: 3\n", 8);
			}
			else
				last_status1 = WEXITSTATUS(status);
		}
		i++;
	}
	return (last_status1);
}

static int	open_pipes(int len_of_exe, int fd[][2], t_cmd *cmd_list)
{
	int	i;

	i = 0;
	while (i < len_of_exe - 1)
	{
		if (pipe(fd[i]) < 0)
		{
			while (--i >= 0)
			{
				close(fd[i][0]);
				close(fd[i][1]);
			}
			free_cmd_list(cmd_list);
			return (0);
		}
		i++;
	}
	return (1);
}

int	work(int len_of_exe, t_cmd *cmd_list, t_env **env, int last_status)
{
	int		fd[len_of_exe - 1][2];
	pid_t	pids[len_of_exe];
	int		last_status2;

	if (!open_pipes(len_of_exe, fd, cmd_list))
		return (1);
	if (!prepare_heredocs(cmd_list))
	{
		close_pipes(len_of_exe - 1, fd);
		free_cmd_list(cmd_list);
		g_sig = 0;
		signals_interactive();
		return (130);
	}
	signals_child();
	create_processes(len_of_exe, fd, cmd_list, env, pids, last_status);
	close_pipes(len_of_exe - 1, fd);
	last_status2 = collect_status(len_of_exe, pids);
	signals_interactive();
	free_cmd_list(cmd_list);
	return (last_status2);
}
