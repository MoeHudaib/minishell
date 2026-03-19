/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:56:53 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:56:54 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_INTERNAL_H
# define PARSE_INTERNAL_H

# include "../organize.h"

/*
** Internal helpers shared between parse.c and parse_utils.c ONLY.
** Do NOT include this anywhere else.
*/

/* parse_utils.c */
t_cmd	*new_cmd(void);

#endif