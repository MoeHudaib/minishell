/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:43:25 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/08/10 11:43:50 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *head, void (*f)(void *))
{
	t_list	*current;

	if (!head || !f)
		return ;
	current = head;
	while (current)
	{
		f(current->content);
		current = current->next;
	}
}
