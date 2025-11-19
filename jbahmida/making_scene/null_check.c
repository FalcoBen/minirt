/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   null_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:28:58 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/19 18:29:00 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static bool	check_cone_object(t_object *obj)
{
	if (!obj->obj->cone || !obj->obj->cone->transform
		|| !obj->obj->cone->inv || !obj->obj->cone->material
		|| !obj->obj->cone->material->color)
		return (true);
	return (false);
}

static bool	check_object_type(t_object *obj)
{
	if (obj->type == T_SPHERE)
		return (check_sphere_object(obj));
	else if (obj->type == T_PLAN)
		return (check_plane_object(obj));
	else if (obj->type == T_CYLINDRE)
		return (check_cylinder_object(obj));
	else if (obj->type == T_CONE)
		return (check_cone_object(obj));
	else
		return (true);
}

static bool	check_objects(t_world *world)
{
	int			i;
	t_object	*obj;

	i = 0;
	while (i < world->obj_num)
	{
		obj = world->objects[i];
		if (!obj || !obj->obj)
		{
			return (true);
		}
		if (check_object_type(obj))
		{
			return (true);
		}
		i++;
	}
	return (false);
}

static void	handle_null_error(t_canva *canva, t_scene *scene)
{
	printf("Error: Null pointer detected in scene data\n");
	if (canva->image)
		mlx_delete_image(canva->mlx, canva->image);
	if (scene)
	{
		free_scene(scene);
		scene = NULL;
	}
	mlx_terminate(canva->mlx);
	alloc(0, true);
	exit(1);
}

void	check_against_null(t_canva *canva, t_scene *scene)
{
	bool	flag;

	flag = false;
	flag = check_camera_and_world(canva->cam, canva->world);
	if (!flag && canva->world->lights)
		flag = check_lights(canva->world);
	if (!flag && canva->world->objects)
		flag = check_objects(canva->world);
	if (flag)
		handle_null_error(canva, scene);
	return ;
}
