/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 18:05:57 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/10 04:33:41 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*dest;
	const unsigned char	*str;

	dest = (unsigned char *)dst;
	str = (const unsigned char *)src;
	if (dest == str)
		return (dst);
	i = 0;
	while (i < n)
	{
		dest[i] = str[i];
		i++;
	}
	return (dst);
}
