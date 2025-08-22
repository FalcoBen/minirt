/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 02:03:17 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/08 22:07:48 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	lens;
	size_t	n;

	lens = ft_strlen(src);
	n = dstsize - 1;
	if (dstsize == 0)
	{
		return (lens);
	}
	while (*src && n > 0)
	{
		*dst++ = *src++;
		n--;
	}
	*dst = '\0';
	return (lens);
}
