/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:53:43 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/08/10 12:54:06 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char		*result;
	size_t		s1_len;
	size_t		s2_len;
	size_t		i;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	i = 0;
	result = malloc(s1_len + s2_len + 1);
	if (!result)
		return (NULL);
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
	{
		result[s1_len + i] = s2[i];
		i++;
	}
	result[s1_len + i] = '\0';
	return (result);
}
