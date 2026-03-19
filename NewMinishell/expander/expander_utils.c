/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:48:30 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:48:30 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

static char	*handle_variable(const char *str, int *i, char **env)
{
	char	*var;
	char	*value;
	int		v;

	v = 0;
	var = malloc(ft_strlen(str) + 1);
	if (!var)
		return (NULL);
	while (is_valid_var_char(str[*i]))
		var[v++] = str[(*i)++];
	var[v] = '\0';
	value = get_env_value(var, env);
	free(var);
	return (value);
}

char	*expand_dollar(const char *str, int *i, int last_status, char **env)
{
	char	*value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (get_exit_status(last_status));
	}
	if (!str[*i] || !is_valid_var_char(str[*i]))
		return (ft_strdup("$"));
	value = handle_variable(str, i, env);
	return (value);
}

char	*get_env_value(char *var, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*get_exit_status(int status)
{
	return (ft_itoa(status));
}
