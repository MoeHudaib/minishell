/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_exe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:48:01 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:48:09 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

void	execute_command(char **cmd, t_env **env, int last_status)
{
	char	*path;

	if (!cmd || !cmd[0])
		exit(1);
	if (is_builtin(cmd[0]))
		exit(exec_builtin(cmd, env, last_status));
	path = build_path(env, cmd[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd[0], STDERR_FILENO);
		exit(127);
	}
	execve(path, cmd, env_to_array(*env));
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(cmd[0], STDERR_FILENO);
	exit(126);
}
