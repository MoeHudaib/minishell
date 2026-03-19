/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:59:13 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:59:14 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_INTERNAL_H
# define HEREDOC_INTERNAL_H

# include "../organize.h"

/*
** Internal helpers shared between prepare_heredocs.c
** and prepare_heredocs_utils.c ONLY.
** Do NOT include this anywhere else.
*/

/* prepare_heredocs.c */
int	read_heredoc(t_redir *redir, int write_fd);
int	copy_pipe(int from_fd, int to_fd);
int	process_heredoc(t_redir *redir, int combined_pipe[2]);
int	fill_combined(t_cmd *cmd, int combined_pipe[2]);
int	assign_combined(t_cmd *cmd, int combined_pipe[2]);

#endif