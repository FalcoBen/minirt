/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 04:47:49 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/10 04:35:26 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char			*str;
	unsigned char				i;
	size_t						count;

	str = (const unsigned char *)s;
	i = (unsigned char)c;
	count = 0;
	while (count < n)
	{
		if (*str == i)
		{
			return ((void *)str);
		}
		count++;
		str++;
	}
	return (NULL);
}
