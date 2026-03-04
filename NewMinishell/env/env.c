#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static t_env	*new_node(char *key, char *value)
{
	t_env	*new_one;

	if (!key || !value)
		return (NULL);
	new_one = malloc(sizeof(t_env));
	if (!new_one)
		return (NULL);
	new_one->key = strdup(key);
	new_one->value = strdup(value);
	new_one->next = NULL;
	return (new_one);
}

static t_env	*add_last(t_env **head, t_env *node)
{
	t_env	*current;

	if (!head || !node)
		return (NULL);
	if (!*head)
	{
		*head = node;
		return (*head);
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = node;
	return (*head);
}
static size_t	get_length(char *line, int flag)
{
	size_t	i;
	size_t	len;

	if (!line)
		return (0);
	i = 0;
	len = 0;
	if (flag)
	{
		while (line[i] && line[i] != '=')
			i++;
		return (i);
	}
	while (line[i] && line[i] != '=')
		i++;
	if (line[i] == '=')
		i++;
	while (line[i])
	{
		len++;
		i++;
	}
	return (len);
}

t_env	*seperate_key_value(char *line, t_env *new_one)
{
	char	*equal;
	char	*key;
	char	*value;
	size_t	key_len;
	size_t	value_len;

	if (!line)
		return (NULL);
	equal = strchr(line, '=');
	if (!equal)
		return (NULL);
	key_len = equal - line;
	value_len = strlen(equal + 1);
	key = malloc(sizeof(char) * (key_len + 1));
	if (!key)
		return (NULL);
	value = malloc(sizeof(char) * (value_len + 1));
	if (!value)
		return (free(key), NULL);
	strncpy(key, line, key_len);
	key[key_len] = '\0';
	strcpy(value, equal + 1);
	new_one = new_node(key, value);
	return (free(key), free(value), new_one);
}

static void	print_list(t_env *head)
{
	t_env	*current;

	if (!head)
	{
		return ;
	}
	current = head;
	while (current)
	{
		printf("Key: %s\nValue: %s\n", current->key, current->value);
		current = current->next;
	}
	printf("\n");
}

t_env    *set_env(char **env)
{
	int		i;
	t_env	*head;
	t_env	*tmp;

	i = 0;
	head = NULL;
	while (env[i])
	{
		tmp = seperate_key_value(env[i], NULL);
		add_last(&head, tmp);
		i++;
	}
    return (head);
}

// int	main(int ac, char **av, char **env)
// {
//     t_env *head;

//     head = set_env(env);
//     print_list(head);
// 	return (0);
// }
