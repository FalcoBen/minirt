/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:47:16 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/03 03:29:42 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_isdigit(int c)
{
	if (!(c >= '0' && c <= '9'))
		return (0);
	return (1);
}

int	count_digits_in_number(char *str, int start)
{
	int	digits;
	int	i;

	digits = 0;
	i = start;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			digits++;
		else
			break ;
		i++;
	}
	return (digits);
}
