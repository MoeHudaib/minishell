/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:02:17 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:02:38 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals_internal.h"

void	sigint_interactive(int sig)
{
	g_sig = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_heredoc(int sig)
{
	g_sig = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	sigint_child(int sig)
{
	g_sig = sig;
	write(STDOUT_FILENO, "\n", 1);
}

void	sigquit_child(int sig)
{
	g_sig = sig;
	write(STDERR_FILENO, "Quit: 3\n", 8);
}
