#include "../organize.h"

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit"));
}

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

static int	apply_single_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == TOKEN_HEREDOC)
	{
		if (redir->fd < 0)
			return (0);
		dup2(redir->fd, STDIN_FILENO);
		close(redir->fd);
		redir->fd = -1;
		return (0);
	}
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

static int	count_commands(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

static int	handle_parent_builtin(t_cmd *cmd_list, t_env **env, int last_status)
{
	int	status;

	status = exec_builtin(cmd_list->args, env, last_status);
	free_cmd_list(cmd_list);
	return (status);
}

static int	wait_child(pid_t pid)
{
	int	status;
	int	last_status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		last_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit: 3\n", 8);
	}
	else
		last_status = WEXITSTATUS(status);
	return (last_status);
}

static void	child_process(t_cmd *cmd_list, t_env **env, int last_status)
{
	signals_reset_child();
	if (apply_redirections(cmd_list))
		exit(1);
	if (!cmd_list->args || !cmd_list->args[0])
		exit(0);
	execute_command(cmd_list->args, env, last_status);
}

static int	wait_and_clean(pid_t pid, t_cmd *cmd_list)
{
    int	last_status;

    last_status = wait_child(pid);
    free_cmd_list(cmd_list);
    signals_interactive();
    return (last_status);
}
static int	run_single(t_cmd *cmd_list, t_env **env, int last_status)
{
	pid_t	pid;

	if (!prepare_heredocs(cmd_list))
	{
		free_cmd_list(cmd_list);
		g_sig = 0;
		signals_interactive();
		return (130);
	}
	signals_child();
	pid = fork();
	if (pid < 0)
	{
		signals_interactive();
		free_cmd_list(cmd_list);
		return (1);
	}
	if (pid == 0)
		child_process(cmd_list, env, last_status);
	return (wait_and_clean(pid, cmd_list));
}


int	are_we_gonna_split(t_lexer *tokens, t_env **env, int last_status)
{
	t_cmd	*cmd_list;
	int		len;

	if (!tokens)
		return (1);
	cmd_list = parse(tokens);
	if (!cmd_list)
		return (1);
	len = count_commands(cmd_list);
	if (len == 1)
	{
		if (cmd_list->args && cmd_list->args[0]
			&& is_parent_builtin(cmd_list->args[0]))
			return (handle_parent_builtin(cmd_list, env, last_status));
		return (run_single(cmd_list, env, last_status));
	}
	return (work(len, cmd_list, env, last_status));
}
