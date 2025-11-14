/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_create_extra.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:49:35 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:43:29 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	init_new_cone(t_cone_fb *new_cone)
{
	new_cone->flag_bump = false;
	new_cone->bump_texture = NULL;
	new_cone->img_path = NULL;
	new_cone->closed_flag = false;
	new_cone->next = NULL;
}

bool	count_data(char **data)
{
	int		i;

	i = 0;
	while (data[i])
		i++;
	if (i > 10)
		return (false);
	return (true);
}

bool	verify_data_cone(char **data, t_cleanup *clean)
{
	double	min;
	double	max;
	double	angle;
	int		flag;

	if (!data[1] || !data[2] || !data[3] || !data[4] || \
			!data[5] || !data[6] || !data[7])
		return (false);
	if (!is_coordinate(data[1]))
		return (false);
	if (!is_coordinate(data[2]))
		return (false);
	min = ft_atoi_double(data[3], clean);
	max = ft_atoi_double(data[4], clean);
	angle = ft_atoi_double(data[5], clean);
	flag = ft_atoi_double(data[6], clean);
	if (clean->flag_exit)
		return (false);
	if (!verify_color(data[7]))
		return (false);
	if (!count_data(data))
		return (false);
	return (true);
}

void	cone_linked_list(t_cone_fb *new_cone, t_scene *scene)
{
	t_cone_fb	*current;

	if (scene->cone == NULL)
	{
		scene->cone = new_cone;
	}
	else
	{
		current = scene->cone;
		while (current->next)
			current = current->next;
		current->next = new_cone;
	}
}
