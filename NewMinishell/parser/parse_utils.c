/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:08:07 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/02/20 19:06:32 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

char	*try_path(const char *cmd, const char *path)//this function is used to combine the cmd with '/' before the combines it with the path Example: cmd, usr/bin -> usr/bin/cmd
{
	char	*new_cmd;
	char	*new_path;

	if (!path || !cmd)
		return (NULL);
	new_cmd = ft_strjoin("/", cmd);
	if (!new_cmd)
		return (NULL);
	new_path = ft_strjoin(path, new_cmd);
	free(new_cmd);
	if (!new_path)
		return (NULL);
	return (new_path);
}

char	*return_path(char **env) // done
{
	int		i;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5) == env[i])
		{
			return (ft_strdup(env[i] + 5));
		}
		i++;
	}
	return (NULL);
}

char	*free_enp(char **enp) // done 
{
	int	i;

	if (!enp)
		return (NULL);
	i = 0;
	while (enp[i])
	{
		free(enp[i]);
		i++;
	}
	free(enp);
	return (NULL);
}
