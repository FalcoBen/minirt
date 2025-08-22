/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:25:27 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/08 18:22:13 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	char	*ft_strcpy(char *dst, const char *src)
{
	char	*first;

	first = dst;
	while (*src)
	{
		*dst++ = *src++;
	}
	*dst = '\0';
	return (first);
}

static	char	*ft_strcat(char *s1, const char *s2)
{
	int	i;
	int	j;

	i = 0;
	while (s1[i])
		i++;
	j = 0;
	while (s2[j])
	{
		s1[i] = s2[j];
		i++;
		j++;
	}
	s1[i] = '\0';
	return (s1);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;
	size_t	lens1;
	size_t	lens2;
	char	*first;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup (s2));
	if (!s2)
		return (ft_strdup (s1));
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	new = (char *)malloc(lens1 + lens2 + 1);
	first = new;
	if (!new)
		return (NULL);
	ft_strcpy(new, s1);
	ft_strcat(new, s2);
	return (first);
}
