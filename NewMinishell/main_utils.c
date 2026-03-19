/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:29:56 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:33:52 by mohammad         ###   ########.fr       */
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
