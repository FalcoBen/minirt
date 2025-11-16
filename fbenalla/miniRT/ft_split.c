/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:33:01 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/13 23:14:51 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

static int	count_words(const char *s, char sep)
{
	int		i;
	int		count;

	count = 0;
	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] != sep && (s[i + 1] == sep || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static int	len_word(const char *s, char sep)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && s[i] != sep)
		i++;
	return (i);
}

static char	**freeing(char **c, int i)
{
	int	index;

	index = 0;
	while (index < i)
	{
		free(c[index]);
		index++;
	}
	free(c);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		i;
	char	*str;

	i = 0;
	if (!s)
		return (NULL);
	str = (char *)s;
	arr = (char **)malloc(sizeof(char *) * (count_words(str, c) + 1));
	if (!arr)
		return (NULL);
	while (i < count_words((char *)s, c))
	{
		while (*str == c)
			str++;
		arr[i] = (char *)malloc(sizeof(char) *(len_word(str, c) + 1));
		if (!arr[i])
			return (freeing(arr, i));
		ft_strlcpy(arr[i], str, (len_word(str, c) + 1));
		str = str + len_word(str, c);
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
