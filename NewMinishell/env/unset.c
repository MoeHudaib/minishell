#include "env.h"

void	unset(char *key, t_env **head)
{
	t_env	*current;
	t_env	*prev;
	t_env	*tmp;

	if (!key || !head || !*head)
		return ;
	current = *head;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key,
				ft_strlen(key) + 1) == 0)
		{
			tmp = current;
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;
			current = current->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
