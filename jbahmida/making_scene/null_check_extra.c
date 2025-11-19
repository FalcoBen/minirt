/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   null_check_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:29:05 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/19 18:29:06 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

bool	check_camera_and_world(t_camera *cam, t_world *world)
{
	if (!cam || !cam->tran || !cam->tran_inv || !world)
		return (true);
	if (world->obj_num < 0 || world->light_num < 0)
		return (true);
	return (false);
}

bool	check_lights(t_world *world)
{
	int	i;

	i = 0;
	while (i < world->light_num)
	{
		if (!world->lights[i] || !world->lights[i]->intensity
			|| !world->lights[i]->position)
			return (true);
		i++;
	}
	return (false);
}

bool	check_sphere_object(t_object *obj)
{
	if (!obj->obj->sphere || !obj->obj->sphere->transform
		|| !obj->obj->sphere->inv || !obj->obj->sphere->material
		|| !obj->obj->sphere->material->color)
		return (true);
	return (false);
}

bool	check_plane_object(t_object *obj)
{
	if (!obj->obj->plane || !obj->obj->plane->transform
		|| !obj->obj->plane->inv || !obj->obj->plane->material
		|| !obj->obj->plane->material->color)
		return (true);
	return (false);
}

bool	check_cylinder_object(t_object *obj)
{
	if (!obj->obj->cylinder || !obj->obj->cylinder->transform
		|| !obj->obj->cylinder->inv || !obj->obj->cylinder->material
		|| !obj->obj->cylinder->material->color)
		return (true);
	return (false);
}
