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

#include "../../MiniRt.h"
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
t_matrix	*rotation_from_to(t_tuple *from, t_tuple *to)
{
	t_matrix	*result;
	t_tuple		v;
	float			factor;
	float			s;
	float			c;
	float			vx[9];
	int			i;
	int			j;
	int			k;

	v = s_cross_product(from, to);
	s = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	c = dot_product(from, to);
	if (s < 0.0001)
	{
		if (c > 0)
			return (identity_matrix(4, 4));
		else
			return (rotation_x((M_PI)));
	}	
	vx[0] = 0.0;
	vx[1] = -v.z;
	vx[2] = v.y;
	vx[3] = v.z;
	vx[4] = 0.0;
	vx[5] = -v.x;
	vx[6] = -v.y;
	vx[7] = v.x;
	vx[8] = 0.0;

	result = identity_matrix(4, 4);
	factor = (1 - c) / (s * s);
	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			k = 0;
			result->matrix[i][j] += vx[i*3 + j];
			while (k < 3)
			{
				result->matrix[i][j] += vx[(i * 3) + k]
					* vx[(k * 3) + j] * factor;
				k++;
			}
			j++;
		}
		i++;
	}

	return (result);
}

t_matrix	*create_cone_transform(t_tuple *position, t_tuple *axis,
	float radius, float height)
{
	t_nor_cone_tran	var;

	var.normalized_axis = *axis;
	s_vec_norm(&var.normalized_axis);
	var.default_y = (t_tuple){0, 1, 0, 0};
	var.rotation = rotation_from_to(&var.default_y, &var.normalized_axis);
	var.scale = scaling(radius, height / 2.0, radius);
	var.trans = translation(position->x, position->y, position->z);
	var.temp = matrix_multi(var.trans, var.rotation);
	var.result = matrix_multi(var.temp, var.scale);
	copy_matrix_contant(var.result);
	return (var.result);
}

void	obj_creator_cone(t_obj *obj, t_cone_fb *src,
			t_ambient_light_fb *ambient_light)
{
	float			height;
	t_tuple			*origin;
	t_tuple			*normal;
	t_matrix		*transform;
	t_cone_params	params;

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
	transform = create_cone_transform(origin, normal, obj->cone->angle, height);
	// transform = create_cone_transform(origin, normal, &params, NULL);
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
