/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:47:05 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:05:43 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

int	ft_exit(char **cmd, int last_status)
{
	int	status;

	write(STDOUT_FILENO, "exit\n", 5);
	if (!cmd[1])
		exit(last_status);
	if (cmd[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (!ft_isnumeric(cmd[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit(2);
	}
	status = ((ft_atoi(cmd[1]) % 256) + 256) % 256;
	exit(status);
}
