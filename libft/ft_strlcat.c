/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:44:40 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/08 21:59:41 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t		lens;
	size_t		lend;
	size_t		n;

	if (dstsize == 0)
		return (ft_strlen(src));
	lens = ft_strlen(src);
	lend = ft_strlen(dst);
	n = dstsize;
	dst = dst + lend;
	n -= lend + 1;
	if (lend >= dstsize)
	{
		return (lens + dstsize);
	}
	while (*src && n--)
	{
		*dst = *src;
		dst++;
		src++;
	}
	*dst = '\0';
	return (lend + lens);
}
