/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:29:56 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/26 12:47:37 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "organize.h"

char	*build_prompt(t_env *env_list)
{
	char	*cwd;
	char	*prompt;

	(void)env_list;
	cwd = NULL;
	if (!cwd)
		return (ft_strdup("minishell$ "));
	prompt = ft_strjoin(cwd, " minishell$ ");
	return (prompt);
}

void	rl_clear_history(void)
{
	clear_history();
}

void	cleanup_shell(t_env *env_list)
{
	if (env_list)
		envclear(&env_list);
	rl_clear_history();
}
