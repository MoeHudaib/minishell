/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:17:10 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:18:49 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORK_H
# define WORK_H

/*
** Forward declarations — work.h is included at the TOP of organize.h
** before t_cmd/t_env are defined, so we use forward structs here.
** The full definitions in organize.h complete them.
*/
typedef struct s_env	t_env;
typedef struct s_cmd	t_cmd;

typedef struct s_fork_args
{
	int		i;
	int		len_of_exe;
	int		(*fd)[2];
	t_cmd	*current;
	t_env	**env;
	int		last_status;
}	t_fork_args;

typedef struct s_proc_args
{
	int		len_of_exe;
	int		(*fd)[2];
	t_cmd	*cmd_list;
	t_env	**env;
	int		last_status;
}	t_proc_args;

/* pipe_utils.c */
void	close_pipes(int len, int fd[][2]);
int		open_pipes(int len_of_exe, int fd[][2], t_cmd *cmd_list);
void	close_all_pipes(int len_of_exe, int fd[][2]);

/* pipe_setup.c */
void	setup_pipe_fds(int i, int len_of_exe, int fd[][2]);

/* fork_child.c */
int		create_processes(t_proc_args *p, pid_t *pids);

/* work.c */
int		work(int len_of_exe, t_cmd *cmd_list,
			t_env **env, int last_status);
int		get_exit_status1(int status);

#endif