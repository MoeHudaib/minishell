/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:04:30 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/02/20 19:06:39 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

/*			
 R_OK (read permission)
 W_OK (write permission)
 X_OK (execute/search permission)
 F_OK (existence test)
 */
# include <sys/wait.h>
# include <unistd.h>
# include <errno.h>
# include <stdio.h> 
# include "../libs/libft/libft.h"

char	*try_path(const char *cmd, const char *path);
char	*return_path(char **env);
char	*free_enp(char **enp);
char	*build_path(char **env, const char *cmd);

#endif
