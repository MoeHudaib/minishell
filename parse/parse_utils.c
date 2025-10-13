/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:08:07 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/10/08 14:25:27 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

char	*try_path(const char *cmd, const char *path)
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

int	is_valid(const char *path, int flag)
{
	if (!path)
		return (-1);
	return (access(path, flag));
}

char	*return_path(char **env)
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

char	*free_enp(char **enp)
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
