/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:47:38 by fbenalla          #+#    #+#             */
/*   Updated: 2025/08/28 12:21:24 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_check(int signe)
{
	if (signe % 2 != 0)
		return (0);
	return (-1);
}

int	ft_atoi_color(const char *str)
{
	int						i;
	int						signe;
	unsigned long long		nb;

	i = 0;
	nb = 0;
	signe = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			signe++;
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9') || str[i] == 44)
	{
		if (nb > (unsigned long long)
			((9223372036854775807 - (str[i] - 48)) / 10))
			return (ft_check(signe));
		nb = nb * 10 + str[i++] - 48;
	}
	if (signe % 2 != 0)
		nb *= -1;
	return ((int)nb);
}


int main()
{
	
}