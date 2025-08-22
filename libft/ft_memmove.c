/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 18:11:41 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/10 04:35:39 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	if (dst == src)
		return (dst);
	if (dst > src)
	{
		while (len--)
			*(unsigned char *)(dst + len) = *((unsigned char *)src + len);
	}
	else
		ft_memcpy(dst, src, len);
	return (dst);
}
