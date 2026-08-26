/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:48:15 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:48:15 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

static int	collect_status(int len_of_exe, pid_t *pids)
{
	int	i;
	int	status;
	int	last;

	i = 0;
	last = 0;
	while (i < len_of_exe)
	{
		waitpid(pids[i], &status, 0);
		if (i == len_of_exe - 1)
			last = get_exit_status1(status);
		i++;
	}
	return (last);
}

static int	run_procs(t_proc_args *p)
{
	pid_t	*pids;
	int		last;

	pids = malloc(sizeof(pid_t) * p->len_of_exe);
	if (!pids)
		return (1);
	signals_child();
	create_processes(p, pids);
	close_pipes(p->len_of_exe - 1, p->fd);
	last = collect_status(p->len_of_exe, pids);
	free(pids);
	return (last);
}

static int	heredoc_fail(int len, int fd[][2], t_cmd *cmd_list)
{
	close_pipes(len - 1, fd);
	free_cmd_list(cmd_list);
	g_sig = 0;
	signals_interactive();
	return (130);
}

int	work(int len_of_exe, t_cmd *cmd_list,
	t_env **env, int last_status)
{
	int			(*fd)[2];
	int			last;
	t_proc_args	p;

	fd = malloc(sizeof(*fd) * (len_of_exe - 1));
	if (!fd)
		return (1);
	if (!open_pipes(len_of_exe, fd, cmd_list))
		return (free(fd), 1);
	if (!prepare_heredocs(cmd_list))
		return (free(fd), heredoc_fail(len_of_exe, fd, cmd_list));
	p = (t_proc_args){len_of_exe, fd, cmd_list, env, last_status};
	last = run_procs(&p);
	signals_interactive();
	free_cmd_list(cmd_list);
	free(fd);
	return (last);
}
