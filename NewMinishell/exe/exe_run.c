/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_run.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:47:44 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:47:45 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_internal.h"

int	wait_child(pid_t pid)
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

void	child_process(t_cmd *cmd_list, t_env **env, int last_status)
{
	signals_reset_child();
	if (apply_redirections(cmd_list))
		exit(1);
	if (!cmd_list->args || !cmd_list->args[0])
		exit(0);
	execute_command(cmd_list->args, env, last_status);
}

int	wait_and_clean(pid_t pid, t_cmd *cmd_list)
{
	int	last_status;

	last_status = wait_child(pid);
	free_cmd_list(cmd_list);
	signals_interactive();
	return (last_status);
}

int	run_single(t_cmd *cmd_list, t_env **env, int last_status)
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
