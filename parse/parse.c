/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 14:58:16 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/10/13 18:56:05 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include <stdio.h>

static char	*check_path_dir(char **dirs, const char *cmd, int i)
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

static char	*find_valid_path(char **dirs, const char *cmd)
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
	path_line = return_path(env);
	if (!path_line)
		return (NULL);
	dirs = ft_split(path_line, ':');
	if (!dirs)
	{
		free(path_line);
		return (NULL);
	}
	free(path_line);
	path = find_valid_path(dirs, cmd);
	return (path);
}
/*
int	main(int ac, char *av[], char *env[])
{
	char	*res = build_path(env, "masooad");
	printf("valid: %s\n", res);
	free(res);
	char *paths = ft_strnstr(env[37], "PATH=",  5);
	printf("%s\n",paths);
	char *path = ft_strtrim(paths, "PATH=");
	printf("%s\n",path);
	char **spl = ft_split(path, ':');
	if (!spl)
		return (1);
	int i = 0;
	char	*path1;
	while (spl[i])
	{
		path1 = ft_strjoin(spl[i], "/ls");
		printf("\n%s\nAccess Found: %d\nAccess exec: %d", path1,
		 access(path1, F_OK), access(path1, X_OK));
		i++;
	}
}
*/
