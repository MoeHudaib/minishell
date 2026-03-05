#include "env.h"

static void	cleansing(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

static void	assign(t_env **current, t_env **prev, t_env **head)
{
	*prev = NULL;
	*current = *head;
}

void	ft_unset(t_env **head, char *key)
{
	t_env	*current;
	t_env	*prev;
	t_env	*tmp;

	if (!key || !head || !*head)
		return ;
	assign(&current, &prev, head);
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			tmp = current;
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;
			current = current->next;
			cleansing(tmp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
