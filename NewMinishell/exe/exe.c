/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:47:47 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/26 12:28:36 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_internal.h"

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit"));
}

static int	count_commands(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

static int	handle_parent_builtin(t_cmd *cmd_list,
	t_env **env, int last_status)
{
	int	status;

	status = exec_builtin(cmd_list->args, env, last_status);
	free_cmd_list(cmd_list);
	return (status);
}

int	are_we_gonna_split(t_lexer *tokens, t_env **env, int last_status)
{
	t_cmd	*cmd_list;
	int		len;

	if (!tokens)
		return (1);
	cmd_list = parse(tokens);
	if (!cmd_list)
		return (1);
	len = count_commands(cmd_list);
	if (len == 1 && cmd_list->args && cmd_list->args[0]
		&& is_parent_builtin(cmd_list->args[0]))
		return (handle_parent_builtin(cmd_list, env, last_status));
	if (len == 1)
		return (run_single(cmd_list, env, last_status));
	return (work(len, cmd_list, env, last_status));
}
