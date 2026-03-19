/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:47:32 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:47:32 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

static void	cleansing(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

static void	remove_node(t_env **head, t_env **prev,
				t_env **current, t_env *tmp)
{
	if (*prev == NULL)
		*head = (*current)->next;
	else
		(*prev)->next = (*current)->next;
	*current = (*current)->next;
	cleansing(tmp);
}

static void	unset_key(t_env **head, char *key)
{
	t_env	*current;
	t_env	*prev;
	t_env	*tmp;

	prev = NULL;
	current = *head;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			tmp = current;
			remove_node(head, &prev, &current, tmp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

int	ft_unset(t_env **head, char **cmd)
{
	int	i;

	i = 1;
	while (cmd[i])
	{
		unset_key(head, cmd[i]);
		i++;
	}
	return (0);
}
