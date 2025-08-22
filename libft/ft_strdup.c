/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:02:19 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/10 04:29:03 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *dst, const char *src)
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

char	*ft_strdup(const char *s1)
{
	size_t	l;
	char	*dest;

	l = ft_strlen(s1) + 1;
	dest = (char *)malloc(l);
	if (!dest)
		return (NULL);
	ft_strcpy(dest, s1);
	return (dest);
}
