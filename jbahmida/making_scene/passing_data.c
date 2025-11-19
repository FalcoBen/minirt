/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passing_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:27 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/16 02:45:33 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"
#include "../../fbenalla/miniRT/parsing.h"

void	loop_lights(t_world *world, t_light_fb *current_light, int *i)
{
	world->lights[*i] = alloc(sizeof(t_light), false);
	world->lights[*i]->intensity = alloc(sizeof(t_color), false);
	world->lights[*i]->intensity->r = current_light->color_light->r / 255.0;
	world->lights[*i]->intensity->g = current_light->color_light->g / 255.0;
	world->lights[*i]->intensity->b = current_light->color_light->b / 255.0;
	world->lights[*i]->intensity->r *= current_light->bright_light;
	world->lights[*i]->intensity->g *= current_light->bright_light;
	world->lights[*i]->intensity->b *= current_light->bright_light;
	world->lights[*i]->position = alloc(sizeof(t_tuple), false);
	world->lights[*i]->position->x = current_light->coor_light->x;
	world->lights[*i]->position->y = current_light->coor_light->y;
	world->lights[*i]->position->z = current_light->coor_light->z;
	world->lights[*i]->position->w = 1;
}

void	light_obj_creator(t_world *world, t_scene *scene)
{
	int			i;
	t_light_fb	*light_count;
	t_light_fb	*current_light;

	light_count = scene->light;
	i = 0;
	world->light_num = 0;
	while (light_count)
	{
		world->light_num++;
		light_count = light_count->next;
	}
	if (world->light_num > 0)
	{
		world->lights = alloc((sizeof(t_light *) * world->light_num), false);
		current_light = scene->light;
		while (current_light && i < world->light_num)
		{
			loop_lights(world, current_light, &i);
			current_light = current_light->next;
			i++;
		}
	}
}

t_world	s_world(t_scene *scene)
{
	t_world	world;
	int		i;

	light_obj_creator(&world, scene);
	world.obj_num = number_of_all_objects(scene);
	world.objects = alloc((sizeof(t_object *) * (world.obj_num + 1)), false);
	i = 0;
	s_world_sphere_constractor(&world, scene, &i);
	s_world_plane_constractor(&world, scene, &i);
	s_world_cylinder_constractor(&world, scene, &i);
	s_world_cone_constractor(&world, scene, &i);
	world.objects[i] = NULL;
	return (world);
}
