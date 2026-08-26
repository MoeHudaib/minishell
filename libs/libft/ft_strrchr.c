/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 13:00:26 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/08/10 13:00:41 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		index;
	int		last_occurness;
	char	chr;

	index = 0;
	chr = (char)c;
	last_occurness = -1;
	while (str[index] != '\0')
	{
		if (str[index] == chr)
			last_occurness = index;
		index++;
	}
	if (c == 0)
		return ((char *)&str[index]);
	if (last_occurness == -1)
		return (NULL);
	str += last_occurness;
	return ((char *)(str));
}
