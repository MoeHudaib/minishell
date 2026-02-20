/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 14:58:16 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/02/20 19:10:20 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include <stdio.h>

static char	*check_path_dir(char **dirs, const char *cmd, int i) // done
{
	char	*path;

	path = try_path(cmd, dirs[i]);
	if (!path)
	{
		free_enp(dirs);
		return (NULL);
	}
	if (access(path, F_OK | X_OK) == 0)
	{
		free_enp(dirs);
		return (path);
	}
	free(path);
	return (NULL);
}

static char	*find_valid_path(char **dirs, const char *cmd) // done 
{
	int		i;
	char	*path;

	if (!dirs || !cmd)
	{
		if (dirs)
			free_enp(dirs);
		return (NULL);
	}
	i = 0;
	while (dirs[i])
	{
		path = check_path_dir(dirs, cmd, i);
		if (path)
			return (path);
		i++;
	}
	free_enp(dirs);
	return (NULL);
}

char	*build_path(char **env, const char *cmd)
{
	char	*path_line;
	char	**dirs;
	char	*path;

	if (!env || !cmd)
		return (NULL);
	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path_line = return_path(env); // it loops through env looking for "PATH=" if existed returns it if not returns null
	if (!path_line)
		return (NULL);
	dirs = ft_split(path_line, ':'); // this splits the "PATH=" into dirs 
	free(path_line);
	if (!dirs)
		return (NULL);
	path = find_valid_path(dirs, cmd); // this loops through all dirs found in "PATH=" in order to look for a valid one then returns it and if none found returns NULL
	return (path);
}

// int	main(int ac, char *av[], char *env[])
// {
// 	char	*res = build_path(env, "echo");
// 	printf("valid: %s\n", res);
// 	free(res);
// }
