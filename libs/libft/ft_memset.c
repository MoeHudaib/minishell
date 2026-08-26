/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:58:35 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/08/10 11:58:59 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *buffer, int value, size_t len)
{
	unsigned char	*a;

	a = (unsigned char *)buffer;
	while (len--)
	{
		*a = (unsigned char)value;
		a++;
	}
	return (buffer);
}
