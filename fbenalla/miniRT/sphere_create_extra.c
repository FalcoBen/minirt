/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_create_extra.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:56:25 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:37:54 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	sphere_linked_list(t_sphere_fb *new_sphere, t_scene *scene)
{
	t_sphere_fb	*current;

	if (scene->sphere == NULL)
	{
		puts("in if spher\n");
		scene->sphere = new_sphere;
	}
	else
	{
		current = scene->sphere;
		while (current->next)
			current = current->next;
		current->next = new_sphere;
	}
}

void	init_new_sphere(t_sphere_fb *new_sphere)
{
	new_sphere->next = NULL;
	new_sphere->bump_texture = NULL;
	new_sphere->img_path = NULL;
	new_sphere->flag_bump = false;
	new_sphere->type = 0;
}

bool	verify_data_sphere(char **data, t_cleanup *clean)
{
	int		i;
	double	raduis;

	i = 0;
	if (!data[1] || !data[2] || !data[3])
		return (false);
	if (!is_coordinate(data[1]))
		return (false);
	raduis = ft_atoi_double(data[2], clean);
	if (clean->flag_exit)
		return (false);
	if (!verify_color(data[3]))
		return (false);
	while (data[i])
		i++;
	if (i > 6)
	{
		printf("alot of arguments in sphere\n");
		return (false);
	}
	return (true);
}
