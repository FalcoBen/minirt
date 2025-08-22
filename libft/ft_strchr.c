/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:30:12 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/08 22:26:21 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	i;

	i = (char )c;
	while (*s != '\0')
	{
		if (*s == i)
		{
			return ((char *)s);
		}
		s++;
	}
	if (i == '\0')
	{
		return ((char *)s);
	}
	return (NULL);
}
