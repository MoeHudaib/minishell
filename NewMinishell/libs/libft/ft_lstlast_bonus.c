/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:44:16 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/08/10 11:44:18 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *head)
{
	t_list	*current;

	if (!head)
		return (NULL);
	current = head;
	while (current->next != NULL)
	{
		current = current->next;
	}
	return (current);
}
