/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:48:28 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/08/10 13:07:53 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int c, size_t n)
{
	const unsigned char	*a;
	size_t				i;
	unsigned char		p;

	a = (const unsigned char *)ptr;
	i = 0;
	p = (unsigned char)c;
	while (i < n)
	{
		if (a[i] == p)
			return ((void *)(a + i));
		i++;
	}
	return (NULL);
}
