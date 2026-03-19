/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:46:33 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:47:37 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_INTERNAL_H
# define EXE_INTERNAL_H

# include "../organize.h"

/*
** Internal helpers shared across simple_exe*.c files ONLY.
** Do NOT include this anywhere else.
*/

/* simple_exe_redir.c */
int		apply_single_redir(t_redir *redir);

/* simple_exe_run.c */
int		wait_child(pid_t pid);
void	child_process(t_cmd *cmd_list, t_env **env, int last_status);
int		wait_and_clean(pid_t pid, t_cmd *cmd_list);
int		run_single(t_cmd *cmd_list, t_env **env, int last_status);

#endif