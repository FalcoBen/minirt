/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passing_data_pl.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:38 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/16 02:46:41 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../fbenalla/miniRT/parsing.h"
#include "../MiniRt.h"

void	checker_textures_pl(t_obj *obj, t_plane_fb *src)
{
	if (src->bump_texture->type == 1)
	{
		obj->plane->material->has_color_texture = true;
		obj->plane->material->color_texture = src->bump_texture;
		obj->plane->material->has_bump_map = false;
		obj->plane->material->bump_strength = 0.1;
	}
	else if (src->bump_texture->type == 2)
	{
		obj->plane->material->has_bump_map = true;
		obj->plane->material->bump_map = src->bump_texture;
		obj->plane->material->has_color_texture = false;
		src->bump_texture->scale = 1.0;
	}
}

void	assigne_plane_object(t_obj *obj, t_plane_fb *src, \
		t_ambient_light_fb *ambient_light)
{
	obj->plane->material->color->r = src->color_plane->r / 255.0;
	obj->plane->material->color->g = src->color_plane->g / 255.0;
	obj->plane->material->color->b = src->color_plane->b / 255.0;
	obj->plane->material->ambient = ambient_light->bright_ambient_light_fb;
	obj->plane->material->diffuse = 0.9;
	obj->plane->material->specular = 0.0;
	obj->plane->material->shininess = 200.0;
	if (src->flag_bump && src->bump_texture)
		checker_textures_pl(obj, src);
	else
	{
		obj->plane->material->has_color_texture = false;
		obj->plane->material->has_bump_map = false;
	}
}

void	assign_coor_and_vects_pl(t_tuple *origin, \
		t_tuple *normal, t_plane_fb *src)
{
	origin->x = src->coor_plane->x;
	origin->y = src->coor_plane->y;
	origin->z = src->coor_plane->z;
	origin->w = 1;
	normal->x = src->vector_plane->x;
	normal->y = src->vector_plane->y;
	normal->z = src->vector_plane->z;
	normal->w = 0;
}

void	obj_creator_plane(t_obj *obj, t_plane_fb *src, \
		t_ambient_light_fb *ambient_light)
{
	t_tuple		*origin;
	t_tuple		*normal;
	t_matrix	*transform;

	if (!obj || !src)
		return ;
	origin = alloc(sizeof(t_tuple), false);
	normal = alloc(sizeof(t_tuple), false);
	assign_coor_and_vects_pl(origin, normal, src);
	plane(&obj->plane, origin, normal);
	transform = create_plane_transform(origin, normal);
	obj->plane->transform = transform;
	obj->plane->inv = matrix_inverte(transform);
	assigne_plane_object(obj, src, ambient_light);
	copy_matrix_contant(obj->plane->inv);
	copy_matrix_contant(obj->plane->transform);
}

void	s_world_plane_constractor(t_world *world, t_scene *scene, int *i)
{
	t_plane_fb	*current_plane;

	current_plane = scene->plane;
	while (current_plane && *i < world->obj_num)
	{
		world->objects[*i] = alloc(sizeof(t_object), false);
		world->objects[*i]->type = T_PLAN;
		world->objects[*i]->obj = alloc(sizeof(t_obj), false);
		obj_creator_plane(world->objects[*i]->obj, \
				current_plane, scene->ambient_light);
		world->objects[*i]->obj->plane->id = *i;
		current_plane = current_plane->next;
		(*i)++;
	}
}
