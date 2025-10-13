/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:56:41 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/08/10 11:57:57 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*a;
	unsigned const char	*b;

	a = (unsigned char *)dest;
	b = (unsigned const char *)src;
	if (a < b)
	{
		while (n--)
		{
			*a++ = *b++;
		}
	}
	else
	{
		while (n--)
		{
			a[n] = b[n];
		}
	}
	return (dest);
}
