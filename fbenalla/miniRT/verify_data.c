/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:23:22 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:36:23 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	check_valid_args(char **data, t_scene *scene, char object)
{
	int	i;

	i = 0;
	while (data[i])
		i++;
	if (object == 'A')
	{
		if (i != 3)
			exit_error("invalid arguments", "in A", scene->cleaner);
	}
	if (object == 'C')
	{
		if (i != 4)
			exit_error("invalid arguments", "in C", scene->cleaner);
	}
	if (object == 'L')
	{
		if (i != 4)
			exit_error("invalid arguments", "in L", scene->cleaner);
	}
}

bool	verify_data_ambient_light(char **data, t_cleanup *clean)
{
	int		i;
	double	ambient;

	i = 0;
	if (!data[1] || !data[2])
		return (false);
	ambient = ft_atoi_double(data[1], clean);
	if (clean->flag_exit)
	{
		printf("333333333333333344444444444444444444\n");
		return (false);
	}
	if (!verify_color(data[2]))
	{
		printf("22222222222222222222222222222222222222\n");
		return (false);
	}
	while (data[i])
		i++;
	if (i > 3)
	{
		printf("alot of arguments in ambient_light\n");
		return (false);
	}
	return (true);
}
