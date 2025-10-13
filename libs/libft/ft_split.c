/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:46:43 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/08/10 12:47:41 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_word_count(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static char	*ft_worddup(const char *start, size_t len)
{
	char	*word;
	size_t	i;

	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static void	ft_free_all(char **arr, int i)
{
	while (--i >= 0)
		free(arr[i]);
	free(arr);
}

static char	**ft_split_words(const char *s, char c, char **arr)
{
	int		i;
	size_t	start;
	size_t	end;

	i = 0;
	end = 0;
	while (s[end])
	{
		if (s[end] != c && (end == 0 || s[end - 1] == c))
			start = end;
		if (s[end] != c && (s[end + 1] == c || s[end + 1] == '\0'))
		{
			arr[i] = ft_worddup(s + start, end - start + 1);
			if (!arr[i++])
			{
				ft_free_all(arr, i);
				return (NULL);
			}
		}
		end++;
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_split(const char *s, char c)
{
	char	**arr;
	int		words;

	if (!s)
		return (NULL);
	words = ft_word_count(s, c);
	arr = malloc((words + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	return (ft_split_words(s, c, arr));
}
