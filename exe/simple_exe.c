/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_exe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:48:01 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/26 13:43:12 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

void	free_clean(t_cmd *cmd_list, t_env **envlist)
{
	envclear(envlist);
	free_cmd_list(cmd_list);
}

void	execute_command(t_cmd *cmd_list, t_env **env, int last_status)
{
	char	*path;

	if (!cmd_list->args || !cmd_list->args[0])
		exit(1);
	if (is_builtin(cmd_list->args[0]))
	{
		last_status = exec_builtin(cmd_list->args, env, last_status);
		free_clean(cmd_list, env);
		exit(last_status);
	}
	path = build_path(env, cmd_list->args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd_list->args[0], STDERR_FILENO);
		free_clean(cmd_list, env);
		exit(127);
	}
	execve(path, cmd_list->args, env_to_array(*env));
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(cmd_list->args[0], STDERR_FILENO);
	free_clean(cmd_list, env);
	exit(126);
}
