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

void	obj_creator_cone(t_obj *obj, t_cone_fb *src, \
		t_ambient_light_fb *ambient_light)
{
	float		height;
	t_tuple		*origin;
	t_tuple		*normal;
	t_matrix	*transform;

	if (!obj || !src)
		return ;
	cone(&obj->cone, NULL, false);
	obj->cone->minimum = src->minimum;
	obj->cone->maximum = src->maximum;
	obj->cone->closed = src->closed_flag;
	origin = alloc(sizeof(t_tuple), false);
	normal = alloc(sizeof(t_tuple), false);
	assign_coor_and_vects_cone(origin, normal, src);
	height = src->maximum - src->minimum;
	obj->cone->origin = origin;
	transform = create_cone_transform_v2(origin, normal, src->angle, \
		height, src->minimum, src->maximum);
	cone(&obj->cone, transform, true);
	assigne_cone_object(obj, src, ambient_light);
	copy_matrix_contant(obj->cone->inv);
	copy_matrix_contant(obj->cone->transform);
}

static t_matrix	*get_axis_alignment_rotation(t_tuple *axis)
{
	t_tuple		default_y = {0, 1, 0, 0};
	t_tuple		norm_axis = *axis;
	float		dot;

	s_vec_norm(&norm_axis);
	dot = dot_product(&default_y, &norm_axis);
	dot = fmax(fmin(dot, 1.0), -1.0);

	if (fabs(dot - 1.0) < 1e-3)
		return (identity_matrix(4, 4));
	else if (fabs(dot + 1.0) < 1e-3)
		return (rotation_x(M_PI));
	else
	{
		t_tuple	cross = *cross_product(&default_y, &norm_axis);
		t_tuple	rot_axis;
		float	angle;

		s_vec_norm(&cross);
		rot_axis = cross;
		angle = acos(dot);
		return (rotation_axis_angle(&rot_axis, angle));
	}
}

static t_matrix	*get_cone_scale_matrix(float angle_degrees, float height)
{
	float	angle_radians = angle_degrees * M_PI / 180.0f;
	float	scale_xz = tanf(angle_radians);

	return (scaling(scale_xz, height / 2.0f, scale_xz));
}

static t_matrix	*get_center_offset_translation(t_tuple *axis, float minimum, float maximum, float height)
{
	float	center_offset = (maximum + minimum) / 2.0f;
	t_tuple	offset_vec = *axis;

	s_vec_norm(&offset_vec);
	s_scalar_mult(&offset_vec, offset_vec, center_offset * (height / 2.0f));

	return (translation(offset_vec.x, offset_vec.y, offset_vec.z));
}

static t_matrix	*get_position_translation(t_tuple *position)
{
	return (translation(position->x, position->y, position->z));
}

t_matrix	*create_cone_transform_v2(t_tuple *position, t_tuple *axis,
		float angle_degrees, float height, float minimum, float maximum)
{
	t_matrix	*rotation;
	t_matrix	*scale;
	t_matrix	*center_offset;
	t_matrix	*trans_to_pos;
	t_matrix	*temp;

	rotation       = get_axis_alignment_rotation(axis);
	scale          = get_cone_scale_matrix(angle_degrees, height);
	center_offset  = get_center_offset_translation(axis, minimum, maximum, height);
	trans_to_pos   = get_position_translation(position);

	// T_position + T_center_offset
	temp   = matrix_multi(trans_to_pos, center_offset);
	// (T_position + T_center_offset) × Rotation
	temp   = matrix_multi(temp, rotation);
	// Final: (T_position + T_center_offset) × R × S
	temp   = matrix_multi(temp, scale);

	copy_matrix_contant(temp);
	return (temp);
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
