/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:29:51 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/08/10 11:31:37 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <limits.h>
#include <stdint.h>

void	*ft_calloc(size_t count, size_t size)
{
	size_t			total;
	unsigned char	*ptr;
	size_t			i;

	if (count != 0 && size > SIZE_MAX / count)
		return (NULL);
	total = count * size;
	ptr = (unsigned char *)malloc(total);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < total)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}
