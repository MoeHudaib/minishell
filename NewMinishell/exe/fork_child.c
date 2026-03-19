/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:47:50 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:47:50 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

static t_fork_args	make_fork_args(int i, t_cmd *cur, t_proc_args *p)
{
	t_fork_args	a;

	a.i = i;
	a.len_of_exe = p->len_of_exe;
	a.fd = p->fd;
	a.current = cur;
	a.env = p->env;
	a.last_status = p->last_status;
	return (a);
}

static int	fork_child(t_fork_args *a)
{
	setup_pipe_fds(a->i, a->len_of_exe, a->fd);
	close_all_pipes(a->len_of_exe, a->fd);
	if (apply_redirections(a->current))
		exit(1);
	execute_command(a->current->args, a->env, a->last_status);
	return (0);
}

static void	handle_child(t_fork_args *a)
{
	signals_reset_child();
	fork_child(a);
}

int	create_processes(t_proc_args *p, pid_t *pids)
{
	int			i;
	t_cmd		*current;
	t_fork_args	a;

	i = 0;
	current = p->cmd_list;
	while (i < p->len_of_exe)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			return (1);
		if (pids[i] == 0)
		{
			a = make_fork_args(i, current, p);
			handle_child(&a);
		}
		current = current->next;
		i++;
	}
	return (0);
}
