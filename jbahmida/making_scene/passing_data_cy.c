/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passing_data_cy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:04 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/16 02:46:08 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../fbenalla/miniRT/parsing.h"
#include "../MiniRt.h"

void	checker_textures_cy(t_obj *obj, t_cylinder_fb *src)
{
	if (src->bump_texture && src->bump_texture->type == 1)
	{
		obj->cylinder->material->has_color_texture = true;
		obj->cylinder->material->color_texture = src->bump_texture;
		obj->cylinder->material->has_bump_map = false;
	}
	else if (src->bump_texture && \
			src->bump_texture->type == 2 && src->bump_texture->image)
	{
		obj->cylinder->material->has_color_texture = false;
		obj->cylinder->material->color_texture = src->bump_texture;
		obj->cylinder->material->has_bump_map = true;
		obj->cylinder->material->bump_map = src->bump_texture;
		obj->cylinder->material->bump_strength = 0.2;
	}
	else
	{
		obj->cylinder->material->has_bump_map = false;
		obj->cylinder->material->bump_map = NULL;
		obj->cylinder->material->has_color_texture = false;
		obj->cylinder->material->color_texture = NULL;
	}
}

void	assigne_cylinder_object(t_obj *obj, t_cylinder_fb *src, \
		t_ambient_light_fb *ambient_light)
{
	obj->cylinder->material->color->r = src->color_cylinder->r / 255.0;
	obj->cylinder->material->color->g = src->color_cylinder->g / 255.0;
	obj->cylinder->material->color->b = src->color_cylinder->b / 255.0;
	obj->cylinder->material->ambient = ambient_light->bright_ambient_light_fb;
	obj->cylinder->material->diffuse = 0.9;
	obj->cylinder->material->specular = 0.9;
	obj->cylinder->material->shininess = 200.0;
	obj->cylinder->minimum = 0.0;
	obj->cylinder->maximum = src->height_cylinder_fb;
	obj->cylinder->closed = src->closed;
	checker_textures_cy(obj, src);
}

void	assign_coor_and_vects_cy(t_tuple *origin, t_tuple *normal, \
		t_cylinder_fb *src)
{
	origin->x = src->coor_cylinder->x;
	origin->y = src->coor_cylinder->y;
	origin->z = src->coor_cylinder->z;
	origin->w = 1;
	normal->x = src->vector_cylinder->x;
	normal->y = src->vector_cylinder->y;
	normal->z = src->vector_cylinder->z;
	normal->w = 0;
}

void	obj_creator_cylinder(t_obj *obj, t_cylinder_fb *src, \
		t_ambient_light_fb *ambient_light)
{
	t_tuple		*origin;
	t_tuple		*normal;
	t_matrix	*transform;
	ld			radius;

	if (!obj || !src)
		return ;
	cylinder(&obj->cylinder, NULL, false);
	radius = src->diameter_cylinder / 2.0;
	origin = alloc(sizeof(t_tuple), false);
	normal = alloc(sizeof(t_tuple), false);
	assign_coor_and_vects_cy(origin, normal, src);
	assigne_cylinder_object(obj, src, ambient_light);
	transform = create_cylinder_transform(origin, normal, \
			radius, src->height_cylinder_fb);
	cylinder(&obj->cylinder, transform, true);
	copy_matrix_contant(obj->cylinder->inv);
	copy_matrix_contant(obj->cylinder->transform);
}

void	s_world_cylinder_constractor(t_world *world, t_scene *scene, int *i)
{
	t_cylinder_fb	*cuurent_cylinder;

	cuurent_cylinder = scene->cylinder;
	while (cuurent_cylinder && *i < world->obj_num)
	{
		world->objects[*i] = alloc(sizeof(t_object), false);
		if (!world->objects[*i])
		{
			fprintf(stderr, "Failed to allocate object  cylinder %d\n", *i);
			exit(1);
		}
		world->objects[*i]->type = T_CYLINDRE;
		world->objects[*i]->obj = alloc(sizeof(t_obj), false);
		if (!world->objects[*i]->obj)
		{
			fprintf(stderr, "Failed to allocate obj  cylinder %d\n", *i);
			exit(1);
		}
		obj_creator_cylinder(world->objects[*i]->obj, \
				cuurent_cylinder, scene->ambient_light);
		world->objects[*i]->obj->cylinder->id = *i;
		cuurent_cylinder = cuurent_cylinder->next;
		(*i)++;
	}
}
