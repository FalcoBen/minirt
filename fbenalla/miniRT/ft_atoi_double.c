/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:47:38 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/03 03:29:42 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

bool	ft_isdigit_double(int c)
{
	if ((c >= '0' && c <= '9') || c == '.')
		return (true);
	printf("[[%c]]\n", c);
	return (false);
}

bool	verify_double_number(char *str, int i)
{
	// int i = 0;
	// printf("]]%s[[\n", str);
	while(str[i])
	{
		// if(!(str[i] >= '0' && str[i] <= '9'))
		if(!ft_isdigit_double(str[i]))
			return false;
		i++;
	}
	return true;
}

double	ft_atoi_double(char *str, t_cleanup *clean)
{
	int						i;
	int						signe;
	unsigned long long		nb;
	double					frac;
	double					divisor;
	if(!str)
	{
		clean->flag_exit = true;
		return -1;
	}
	i = 0;
	nb = 0;
	frac = 0;
	divisor = 10.0;
	signe = 0;
	// if(!verify_color(str))
	// {
	// 	clean->flag_exit = true;
	// 	return -1;
	// }
	
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			signe++;
		i++;
	}
	if(!verify_double_number(str, i))
	{
		puts("here\n");
		clean->flag_exit = true;
		return -1;
	}
	// if (non_num_chara(str, i))
	// {
	// 	// exit_error("invalid character", NULL, s);
	// }
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