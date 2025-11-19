/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:47:38 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:57:15 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	ft_isdigit_double(int c)
{
	if ((c >= '0' && c <= '9') || c == '.')
		return (true);
	printf("[[%c]]\n", c);
	return (false);
}

bool	verify_double_number(char *str, int i)
{
	if (!count_dot(str, i))
	{
		return (false);
	}
	while (str[i])
	{
		if (!ft_isdigit_double(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void	init_parse(t_parse *p)
{
	p->i = 0;
	p->signe = 0;
	p->nb = 0;
	p->frac = 0;
	p->divisor = 10.0;
}

void	parse_number(char *str, t_parse *p)
{
	while (str[p->i] >= '0' && str[p->i] <= '9')
		p->nb = p->nb * 10 + (str[p->i++] - '0');
	if (str[p->i] == '.')
	{
		p->i++;
		while (str[p->i] >= '0' && str[p->i] <= '9')
		{
			p->frac += (double)(str[p->i++] - '0') / p->divisor;
			p->divisor *= 10.0;
		}
	}
}

double	ft_atoi_double(char *str, t_cleanup *clean)
{
	t_parse	p;

	if (!str)
		return (clean->flag_exit = true, -1);
	init_parse(&p);
	while ((str[p.i] >= 9 && str[p.i] <= 13) || str[p.i] == 32)
		p.i++;
	if (str[p.i] == '+' || str[p.i] == '-')
		if (str[p.i++] == '-')
			p.signe++;
	if (!verify_double_number(str, p.i))
		return (clean->flag_exit = true, -1);
	parse_number(str, &p);
	if (p.signe % 2 != 0)
		return (-(p.nb + p.frac));
	return (p.nb + p.frac);
}
