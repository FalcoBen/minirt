/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:47:38 by fbenalla          #+#    #+#             */
/*   Updated: 2025/08/28 12:43:14 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static	int	ft_check(int signe)
{
	if (signe % 2 != 0)
		return (0);
	return (-1);
}

bool	non_num_chara(char *str, int i)
{
	int	j;

	j = i;
	while (str[j])
	{
		if (!((str[j] >= '0' && str[j] <= '9') || str[j] == '.' || str[j] == ' ' || str[j] == '\t' || str[j] == '\n'))
				return (true);
		j++;
	}
	// skip_spaces(str, &j);
	return (false);
}
// void s()
// {
// 	system("leaks miniRT scene.rt");
// }


double	ft_atoi_double(char *str)
{
	int						i;
	int						signe;
	unsigned long long		nb;
	double					frac;
	double					divisor;

	i = 0;
	nb = 0;
	frac = 0;
	divisor = 10.0;
	signe = 0;

	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			signe++;
		i++;
	}
	if (non_num_chara(str, i))
	{
		//exit_error("invalid character", NULL);
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] == '.')
	{
		i++;
		while (str[i] >= '0' && str[i] <= '9')
		{
			frac += (double)(str[i] - '0') / divisor;
			divisor *= 10.0;
			i++;
		}
	}
	if (signe % 2 != 0)
		return -(nb + frac);
	return (nb + frac);
}


// int main()
// {
// 	char *str = "80";
// 	printf("[%f]\n", ft_atoi_double(str));
	
	
// }