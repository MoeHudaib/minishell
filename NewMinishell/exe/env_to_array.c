#include "exe.h"

static int	env_size(t_env *head)
{
	int	count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

static char	*join_env(char *key, char *value)
{
	char	*env;
	size_t	len;

	len = ft_strlen(key) + ft_strlen(value) + 2;
	env = malloc(len);
	if (!env)
		return (NULL);
	ft_strlcpy(env, key, len);
	ft_strlcat(env, "=", len);
	ft_strlcat(env, value, len);
	return (env);
}

char	**env_to_array(t_env *head)
{
	char	**env;
	int		i;

	env = malloc(sizeof(char *) * (env_size(head) + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (head)
	{
		env[i] = join_env(head->key, head->value);
		if (!env[i])
			return (NULL);
		head = head->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}