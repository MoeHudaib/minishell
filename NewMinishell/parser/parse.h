/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:04:30 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/03/13 05:52:45 by mohammad         ###   ########.fr       */
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

# include "../lexer/lexer.h"
# include <fcntl.h>

#   include <signal.h>

typedef struct s_parse
{
    char **cmd;
    t_env   *next;
}   t_parse;

char	*try_path(char *cmd, char *path);
char	*return_path(t_env *head);
char	*free_enp(char **enp);
char	*build_path(t_env **head, char *cmd);
int has_complex(t_lexer *tokens);

#endif
