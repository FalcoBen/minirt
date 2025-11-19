/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passing_data_cone.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:47:12 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/16 02:47:14 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniRt.h"
#include "../../fbenalla/miniRT/parsing.h"

void	checker_textures_cone(t_obj *obj, t_cone_fb *src)
{
	if (src->bump_texture->type == 1)
	{
		obj->cone->material->has_color_texture = true;
		obj->cone->material->color_texture = src->bump_texture;
		obj->cone->material->has_bump_map = false;
	}
	else if (src->bump_texture->type == 2)
	{
		obj->cone->material->has_bump_map = true;
		obj->cone->material->bump_map = src->bump_texture;
		obj->cone->material->bump_strength = 0.3;
		obj->cone->material->has_color_texture = false;
	}
}

void	assigne_cone_object(t_obj *obj, t_cone_fb *src, \
		t_ambient_light_fb *ambient_light)
{
	obj->cone->material->color->r = src->color_cone->r / 255.0;
	obj->cone->material->color->g = src->color_cone->g / 255.0;
	obj->cone->material->color->b = src->color_cone->b / 255.0;
	obj->cone->material->ambient = ambient_light->bright_ambient_light_fb;
	obj->cone->material->diffuse = 0.9;
	obj->cone->material->specular = 0.9;
	obj->cone->material->shininess = 200.0;
	obj->cone->minimum = src->minimum;
	obj->cone->maximum = src->maximum;
	obj->cone->closed = src->closed_flag;
	if (src->flag_bump && src->bump_texture)
	{
		checker_textures_cone(obj, src);
	}
	else
	{
		obj->cone->material->has_color_texture = false;
		obj->cone->material->has_bump_map = false;
	}
}

void	assign_coor_and_vects_cone(t_tuple *origin, \
		t_tuple *normal, t_cone_fb *src)
{
	origin->x = src->coor_cone->x;
	origin->y = src->coor_cone->y;
	origin->z = src->coor_cone->z;
	origin->w = 1;
	normal->x = src->vector_cone->x;
	normal->y = src->vector_cone->y;
	normal->z = src->vector_cone->z;
	normal->w = 0;
}

void	obj_creator_cone(t_obj *obj, t_cone_fb *src,
			t_ambient_light_fb *ambient_light)
{
	float			height;
	t_tuple		 	*origin;
	t_tuple		 	*normal;
	t_matrix		*transform;
	t_cone_params   params;

	cone(&obj->cone, NULL, false);
	obj->cone->minimum = src->minimum;
	obj->cone->maximum = src->maximum;
	obj->cone->closed = src->closed_flag;
	origin = alloc(sizeof(t_tuple), false);
	normal = alloc(sizeof(t_tuple), false);
	assign_coor_and_vects_cone(origin, normal, src);
	height = src->maximum - src->minimum;
	params.angle_degrees = src->angle;
	params.height = height;
	params.minimum = src->minimum;
	params.maximum = src->maximum;
	obj->cone->origin = origin;
	transform = create_cone_transform_v2(origin, normal, &params, NULL);
	cone(&obj->cone, transform, true);
	assigne_cone_object(obj, src, ambient_light);
	copy_matrix_contant(obj->cone->inv);
	copy_matrix_contant(obj->cone->transform);
}

void	s_world_cone_constractor(t_world *world, t_scene *scene, int *i)
{
	t_cone_fb	*current_cone;

	current_cone = scene->cone;
	while (current_cone && *i < world->obj_num)
	{
		world->objects[*i] = alloc(sizeof(t_object), false);
		world->objects[*i]->type = T_CONE;
		world->objects[*i]->obj = alloc(sizeof(t_obj), false);
		obj_creator_cone(world->objects[*i]->obj, \
				current_cone, scene->ambient_light);
		world->objects[*i]->obj->cone->id = *i;
		current_cone = current_cone->next;
		(*i)++;
	}
}
