/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_create_extra.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:51:45 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:38:01 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	verify_data_cylinder(char **data, t_cleanup *clean)
{
	int		i;
	double	height;
	double	diameter;
	int		check;

	i = 0;
	if (!data[1] || !data[2] || !data[3] || !data[4] || !data[5] || !data[6])
		return (false);
	if (!is_coordinate(data[1]))
		return (false);
	if (!is_coordinate(data[2]))
		return (false);
	height = ft_atoi_double(data[3], clean);
	diameter = ft_atoi_double(data[4], clean);
	check = ft_atoi_double(data[5], clean);
	if (clean->flag_exit)
		return (false);
	if (!verify_color(data[6]))
		return (false);
	while (data[i])
		i++;
	if (i > 9)
		return (false);
	return (true);
}

void	cylinder_linked_list(t_cylinder_fb *new_cylinder, t_scene *scene)
{
	t_cylinder_fb	*current;

	if (scene->cylinder == NULL)
		scene->cylinder = new_cylinder;
	else
	{
		current = scene->cylinder;
		while (current->next)
			current = current->next;
		current->next = new_cylinder;
	}
}

void	init_new_cylinder(t_cylinder_fb *new_cylinder)
{
	new_cylinder->flag_bump = false;
	new_cylinder->closed = false;
	new_cylinder->bump_texture = NULL;
	new_cylinder->img_path = NULL;
	new_cylinder->next = NULL;
}
