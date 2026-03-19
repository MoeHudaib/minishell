/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:49:15 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:49:16 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

static char	*check_path_dir(char **dirs, char *cmd, int i)
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

static char	*find_valid_path(char **dirs, char *cmd)
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

char	*build_path(t_env **head, char *cmd)
{
	char	*path_line;
	char	**dirs;
	char	*path;

	if (!head || !cmd)
		return (NULL);
	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path_line = return_path(*head);
	if (!path_line)
		return (NULL);
	dirs = ft_split(path_line, ':');
	free(path_line);
	if (!dirs)
		return (NULL);
	path = find_valid_path(dirs, cmd);
	return (path);
}

// int	main(int ac, char *av[], char *env[])
// {
// 	char	*res = build_path(env, "echo");
// 	printf("valid: %s\n", res);
// 	free(res);
// }
