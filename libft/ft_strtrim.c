/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:18:00 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/08 22:30:03 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	start(const char *s1, const char *set)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(s1);
	i = 0;
	while (i < len)
	{
		if (ft_strchr(set, s1[i]) == 0)
			break ;
		i++;
	}
	return (i);
}

static	size_t	end(const char *s1, const char *set)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(s1);
	i = 0;
	while (i < len)
	{
		if (ft_strchr(set, s1[len - i - 1]) == 0)
			break ;
		i++;
	}
	return (len - i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new;
	size_t	first;
	size_t	last;
	size_t	len;
	size_t	i;

	if (!s1 || !set)
		return (NULL);
	first = start(s1, set);
	last = end(s1, set);
	if (first > last)
		return (ft_strdup(""));
	len = last - first;
	new = (char *)malloc(len + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (last > first)
	{
		new[i] = s1[first];
		first++;
		i++;
	}
	new[i] = '\0';
	return (new);
}
