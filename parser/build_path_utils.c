/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:49:10 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:49:11 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

char	*try_path(char *cmd, char *path)
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

char	*return_path(t_env *head)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strnstr(current->key, "PATH", 5) == current->key)
		{
			return (ft_strdup(current->value));
		}
		current = current->next;
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
