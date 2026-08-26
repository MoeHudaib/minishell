/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:47:54 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:47:54 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

static void	setup_first_cmd(int i, int fd[][2])
{
	dup2(fd[i][1], STDOUT_FILENO);
}

static void	setup_last_cmd(int i, int fd[][2])
{
	dup2(fd[i - 1][0], STDIN_FILENO);
}

static void	setup_middle_cmd(int i, int fd[][2])
{
	dup2(fd[i - 1][0], STDIN_FILENO);
	dup2(fd[i][1], STDOUT_FILENO);
}

void	setup_pipe_fds(int i, int len_of_exe, int fd[][2])
{
	if (i == 0)
		setup_first_cmd(i, fd);
	else if (i == len_of_exe - 1)
		setup_last_cmd(i, fd);
	else
		setup_middle_cmd(i, fd);
}
