/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 05:47:23 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 05:47:23 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../organize.h"

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
