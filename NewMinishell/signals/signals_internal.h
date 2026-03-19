/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:01:57 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:01:57 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_INTERNAL_H
# define SIGNALS_INTERNAL_H

# include "../organize.h"

/*
** Internal signal handlers used by signals.c ONLY.
** Do NOT include this anywhere else.
*/

void	sigint_interactive(int sig);
void	sigint_heredoc(int sig);
void	sigint_child(int sig);
void	sigquit_child(int sig);

#endif