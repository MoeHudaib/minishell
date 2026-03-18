#include "../organize.h"

static t_env	*new_node(char *key, char *value)
{
	t_env	*new_one;

	if (!key || !value)
		return (NULL);
	new_one = malloc(sizeof(t_env));
	if (!new_one)
		return (NULL);
	new_one->key = key;
	new_one->value = value;
	new_one->next = NULL;
	return (new_one);
}

t_env	*update_env(char *key, char *value, t_env **head)
{
	t_env		*current;
	size_t		len;
	char		*tmp;

	len = ft_strlen(key);
	current = *head;
	while (current)
	{
		if (strncmp(current->key, key, len) == 0)
		{
			tmp = current->value;
			current->value = ft_strdup(value);
			free(tmp);
		}
		current = current->next;
	}
	return (*head);
}

t_env	*add_last(t_env **head, t_env *node)
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
void	envclear(t_env **head)
{
	t_env	*current;
	t_env	*tmp;

	if (!head || !*head)
		return ;
	current = *head;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	*head = NULL;
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
	equal = ft_strchr(line, '=');
	if (!equal)
		return (NULL);
	key_len = equal - line;
	value_len = ft_strlen(equal + 1);
	key = malloc(key_len + 1);
	if (!key)
		return (NULL);
	value = malloc(value_len + 1);
	if (!value)
		return (free(key), NULL);
	ft_strlcpy(key, line, key_len + 1);
	ft_strlcpy(value, equal + 1, value_len + 1);
	new_one = new_node(key, value);
	return (new_one);
}

int	print_list(t_env *head)
{
	t_env	*current;

	if (!head)
	{
		return (1);
	}
	current = head;
	while (current)
	{
		printf("Key: %s\nValue: %s\n", current->key, current->value);
		current = current->next;
	}
	printf("\n");
	return (0);
}

t_env	*set_env(char **env)
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
//	 t_env *head;
// 	char buf[1024];

//	 head = set_env(env);
//	 print_list(head);
// 	char *pwd = getcwd(buf, 1024);
// 	printf("\n%s\n", pwd);
// 	char *old_pwd = getcwd(buf, 1024);
// 	printf("\n%s\n", old_pwd);
// 	printf("\n%s\n", buf);
// 	update_env("PWD", "HELLO", &head);
// 	print_list(head);
// 	envclear(&head);
// 	return (0);
// }
