/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:47:38 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/13 22:21:40 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static	int	ft_check(int signe)
{
	if (signe % 2 != 0)
		return (0);
	return (-1);
}

int	ft_atoi(char *str)
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
	while (str[i] >= '0' && str[i] <= '9')
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

int	count_comma(char *str)
{
	int	i;
	int	comma;

	i = 0;
	comma = 0;
	while (str[i])
	{
		if (str[i] == ',')
			comma++;
		i++;
	}
	return (comma);
}

bool	verify_color(char *str)
{
	char	**splitted;
	int		i;
	int		nb;

	splitted = ft_split(str, ',');
	i = 0;
	while (splitted[i])
	{
		nb = ft_atoi(splitted[i]);
		if (nb > 255 || nb < 0)
		{
			ft_free_split(splitted);
			return (false);
		}
		i++;
	}
	ft_free_split(splitted);
	return (true);
}
bool	check_after_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (count_comma(str[i]) != 2)
			return (false);
		if (!verify_color(str[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	verify_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	ft_atoi_color(char *str, char *scene)
{
	int						i;
	int						signe;
	unsigned long long		nb;
	int						j;

	j = 0;
	if (!str)
		return (-1);
	// if(count_comma(str) != 2)
	// {
	// 	puts("kkkkkkkkkkkkkkkk\n");
	// 	return (-1);
	// }
	if (!verify_color(str))
		return (-1);
	if (!verify_number(str))
	{
		puts("here\n");
		return (-1);
	}
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

