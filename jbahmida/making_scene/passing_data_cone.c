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

// void	obj_creator_cone(t_obj *obj, t_cone_fb *src, \
// 		t_ambient_light_fb *ambient_light)
// {
// 	float		radius;
// 	float		height;
// 	t_tuple		*origin;
// 	t_tuple		*normal;
// 	t_matrix	*transform;

// 	if (!obj || !src)
// 		return ;
// 	cone(&obj->cone, NULL, false);
// 	radius = src->maximum / 2.0;
// 	obj->cone->angle = src->angle;
// 	origin = alloc(sizeof(t_tuple), false);
// 	normal = alloc(sizeof(t_tuple), false);
// 	assign_coor_and_vects_cone(origin, normal, src);
// 	height = src->maximum - src->minimum;
// 	obj->cone->origin = origin;
// 	transform = create_cone_transform(origin, normal, obj->cone->angle, height);
// 	cone(&obj->cone, transform, true);
// 	assigne_cone_object(obj, src, ambient_light);
// 	copy_matrix_contant(obj->cone->inv);
// 	copy_matrix_contant(obj->cone->transform);
// }
void	obj_creator_cone(t_obj *obj, t_cone_fb *src, \
		t_ambient_light_fb *ambient_light)
{
	float		height;
	t_tuple		*origin;
	t_tuple		*normal;
	t_matrix	*transform;

	if (!obj || !src)
		return ;
	
	// printf("\n### CREATING CONE ###\n");
	// printf("Position: (%f, %f, %f)\n", 
	// 	src->coor_cone->x, src->coor_cone->y, src->coor_cone->z);
	// printf("Direction: (%f, %f, %f)\n",
	// 	src->vector_cone->x, src->vector_cone->y, src->vector_cone->z);
	// printf("Min: %f, Max: %f, Angle: %f, Closed: %d\n",
	// 	src->minimum, src->maximum, src->angle, src->closed_flag);
	
	cone(&obj->cone, NULL, false);
	
	// First set the min/max on the cone object
	obj->cone->minimum = src->minimum;
	obj->cone->maximum = src->maximum;
	obj->cone->closed = src->closed_flag;
	
	origin = alloc(sizeof(t_tuple), false);
	normal = alloc(sizeof(t_tuple), false);
	assign_coor_and_vects_cone(origin, normal, src);
	height = src->maximum - src->minimum;
	obj->cone->origin = origin;
	
	// Now create transform with access to min/max
	transform = create_cone_transform_v2(origin, normal, src->angle, 
		height, src->minimum, src->maximum);
	
	cone(&obj->cone, transform, true);
	assigne_cone_object(obj, src, ambient_light);
	
	// printf("Final cone: min=%f, max=%f, closed=%d\n",
	// 	obj->cone->minimum, obj->cone->maximum, obj->cone->closed);
	// printf("### END CONE CREATION ###\n\n");
	
	copy_matrix_contant(obj->cone->inv);
	copy_matrix_contant(obj->cone->transform);
}

t_matrix	*create_cone_transform_v2(t_tuple *position, t_tuple *axis,
	float angle_degrees, float height, float minimum, float maximum)
{
	t_nor_cone_tran	var;
	float			angle_radians;
	float			scale_xz;
	float			center_offset;
	t_tuple			offset_vec;

	var.normalized_axis = *axis;
	s_vec_norm(&var.normalized_axis);
	
	var.default_y = (t_tuple){0, 1, 0, 0};
	var.dot = dot_product(&var.default_y, &var.normalized_axis);
	var.dot = fmax(fmin(var.dot, 1.0), -1.0);
	
	var.rotation = NULL;
	if (fabs(var.dot - 1.0) < 1e-3)
		var.rotation = identity_matrix(4, 4);
	else if (fabs(var.dot + 1.0) < 1e-3)
		var.rotation = rotation_x(M_PI);
	else
	{
		var.rot_axis = *cross_product(&var.default_y, &var.normalized_axis);
		s_vec_norm(&var.rot_axis);
		var.angle = acos(var.dot);
		var.rotation = rotation_axis_angle(&var.rot_axis, var.angle);
	}
	
	angle_radians = angle_degrees * M_PI / 180.0;
	scale_xz = tanf(angle_radians);
	
	var.scale = scaling(scale_xz, height / 2.0, scale_xz);
	
	// The cone in object space goes from minimum to maximum
	// Its center is at (minimum + maximum) / 2
	center_offset = (maximum + minimum) / 2.0;
	offset_vec = var.normalized_axis;
	s_scalar_mult(&offset_vec, offset_vec, center_offset * (height / 2.0));
	
	var.trans = translation(
		position->x + offset_vec.x, 
		position->y + offset_vec.y, 
		position->z + offset_vec.z
	);
	
	var.temp = matrix_multi(var.trans, var.rotation);
	var.result = matrix_multi(var.temp, var.scale);
	copy_matrix_contant(var.result);
	return (var.result);
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
