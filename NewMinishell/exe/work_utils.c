/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:48:11 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:48:12 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

int	get_sig_status(int status)
{
	if (WTERMSIG(status) == SIGQUIT)
		write(STDERR_FILENO, "Quit: 3\n", 8);
	return (128 + WTERMSIG(status));
}

int	get_exit_status1(int status)
{
	if (WIFSIGNALED(status))
		return (get_sig_status(status));
	return (WEXITSTATUS(status));
}
