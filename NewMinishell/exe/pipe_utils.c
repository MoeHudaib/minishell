/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:47:58 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:47:59 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

void	close_pipes(int len, int fd[][2])
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

static int	open_one_pipe(int i, int fd[][2], t_cmd *cmd_list)
{
	if (pipe(fd[i]) >= 0)
		return (1);
	while (--i >= 0)
	{
		close(fd[i][0]);
		close(fd[i][1]);
	}
	free_cmd_list(cmd_list);
	return (0);
}

int	open_pipes(int len_of_exe, int fd[][2], t_cmd *cmd_list)
{
	int	i;

	i = 0;
	while (i < len_of_exe - 1)
	{
		if (!open_one_pipe(i, fd, cmd_list))
			return (0);
		i++;
	}
	return (1);
}

void	close_all_pipes(int len_of_exe, int fd[][2])
{
	int	j;

	j = 0;
	while (j < len_of_exe - 1)
	{
		close(fd[j][0]);
		close(fd[j][1]);
		j++;
	}
}
