/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 03:03:15 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/08 22:24:44 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	i;
	char	*end;
	int		len;
	int		flag;

	flag = ft_strlen(s);
	i = (char )c;
	len = ft_strlen(s);
	end = (char *)(s + len);
	while (flag >= 0)
	{
		if (*end == i)
			return (end);
		flag--;
		end--;
	}
	return (NULL);
}
