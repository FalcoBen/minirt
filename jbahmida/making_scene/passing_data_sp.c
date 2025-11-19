/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passing_data_sp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:59 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/16 02:47:04 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../fbenalla/miniRT/parsing.h"
#include "../../MiniRt.h"

void	checker_textures_sp(t_obj *obj, t_sphere_fb *src)
{
	if (src->bump_texture->type == 1)
	{
		obj->sphere->material->has_color_texture = true;
		obj->sphere->material->color_texture = src->bump_texture;
		obj->sphere->material->has_bump_map = false;
	}
	else if (src->bump_texture->type == 2)
	{
		obj->sphere->material->has_bump_map = true;
		obj->sphere->material->bump_map = src->bump_texture;
		obj->sphere->material->bump_strength = 0.3;
		obj->sphere->material->has_color_texture = false;
	}
}

void	assign_spher_object(t_obj *obj, t_sphere_fb *src, \
		t_ambient_light_fb *ambient_light)
{
	obj->sphere->material->color->r = src->color_sphere->r / 255.0;
	obj->sphere->material->color->g = src->color_sphere->g / 255.0;
	obj->sphere->material->color->b = src->color_sphere->b / 255.0;
	obj->sphere->material->ambient = ambient_light->bright_ambient_light_fb;
	obj->sphere->material->diffuse = 0.9;
	obj->sphere->material->specular = 0.9;
	obj->sphere->material->shininess = 200.0;
	if (src->flag_bump && src->bump_texture)
		checker_textures_sp(obj, src);
	else
	{
		obj->sphere->material->has_color_texture = false;
		obj->sphere->material->has_bump_map = false;
		obj->sphere->material->bump_map = NULL;
	}
}

void	obj_creator_sphere(t_obj *obj, t_sphere_fb *src, \
		t_ambient_light_fb *ambient_light)
{
	float		radius;
	t_matrix	*trans;
	t_matrix	*scale;
	t_matrix	*transform;

	if (!obj || !src)
		return ;
	sphere(&obj->sphere, NULL, false);
	radius = src->diameter_sphere / 2.0;
	trans = translation(src->coor_sphere->x, \
								src->coor_sphere->y, \
								src->coor_sphere->z);
	scale = scaling(radius, radius, radius);
	transform = matrix_multi(trans, scale);
	sphere(&obj->sphere, transform, true);
	assign_spher_object(obj, src, ambient_light);
	copy_matrix_contant(obj->sphere->inv);
	copy_matrix_contant(obj->sphere->transform);
}

void	s_world_sphere_constractor(t_world *world, t_scene *scene, int *i)
{
	t_sphere_fb	*current_sphere;

	current_sphere = scene->sphere;
	while (current_sphere && *i < world->obj_num)
	{
		world->objects[*i] = alloc(sizeof(t_object), false);
		world->objects[*i]->type = T_SPHERE;
		world->objects[*i]->obj = alloc(sizeof(t_obj), false);
		obj_creator_sphere(world->objects[*i]->obj, \
				current_sphere, scene->ambient_light);
		world->objects[*i]->obj->sphere->id = *i;
		current_sphere = current_sphere->next;
		(*i)++;
	}
}
