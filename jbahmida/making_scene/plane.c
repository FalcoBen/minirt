/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 21:45:31 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/14 04:48:16 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	plane(t_plane **plane, t_tuple *origin, t_tuple *vect)
{
	static int	id;

	id = 0;
	if (!plane)
		return ;
	(*plane) = alloc(sizeof(t_plane), false);
	(*plane)->transform = identity_matrix(4, 4);
	(*plane)->inv = identity_matrix(4, 4);
	(*plane)->id = id;
	(*plane)->material = material();
	(*plane)->origin = origin;
	(*plane)->vect = vect;

	id++;
}

t_matrix	*create_plane_transform(t_tuple *origin, t_tuple *normal)
{
	t_tuple normalized_normal;
	t_tuple default_y;
	ld dot;
	t_matrix *rotation;
	t_matrix *trans;
	t_matrix *result;
	t_tuple axis;
	ld angle;

	normalized_normal = *normal;
	s_vec_norm(&normalized_normal);
	default_y = (t_tuple){0, 1, 0, 0};
	dot = dot_product(&default_y, &normalized_normal);
	rotation = NULL;
	if (fabsf(dot - 1.0) < 0.001)
		rotation = identity_matrix(4, 4);
	else if (fabsf(dot + 1.0) < 0.001)
		rotation = rotation_x(M_PI);
	else
	{
		axis = *cross_product(&default_y, &normalized_normal);
		s_vec_norm(&axis);
		angle = acos(dot);
		if (fabsf(normalized_normal.x) > 0.999)
		{
			if (normalized_normal.x > 0)
				rotation = rotation_z(-M_PI/2);
			else
				rotation = rotation_z(M_PI/2);
		}
		else if (fabsf(normalized_normal.z) > 0.999)
		{
			if (normalized_normal.z > 0)
				rotation = rotation_x(M_PI/2);
			else
				rotation = rotation_x(-M_PI/2);
		}
		else
			rotation = identity_matrix(4, 4);
	}
	trans = translation(origin->x, origin->y, origin->z);
	result = matrix_multi(trans, rotation);
	copy_matrix_contant(result);
	return (result);
}


void	plane_intersct(t_stack_intersections *xs,
	t_plane *plane, t_stack_ray *ray)
{
	t_stack_ray	local_ray;
	ld			t;

	xs->count = 0;
	xs->existence = false;
	local_ray = s_transform(ray, plane->inv);
	if (fabsf(local_ray.vect.y) < FLT_EPSILON)
	{
		xs->world_inters_list = NULL;
		return ;
	}
	t = -local_ray.point.y / local_ray.vect.y;
	xs->inters_list[0].type = T_PLAN;
	xs->inters_list[0].inters_value = t;
	xs->inters_list[0].sphere = NULL;
	xs->inters_list[0].cylinder = NULL;
	xs->inters_list[0].cone = NULL;
	xs->inters_list[0].plane = plane;
	xs->inters_list[0].existence = true;
	xs->count = 1;
	xs->existence = true;
}


void	s_plane_normal(t_tuple *norm, t_plane *plane, t_tuple *point)
{
	t_tuple		local_normal;
	t_matrix	transpose;
	t_matrix	world_normal_matrix;

	local_normal = (t_tuple){0, 1, 0, 0};
	transpose = s_matrix_transpos(plane->inv);
	world_normal_matrix = s_matrix_tuple(&transpose, &local_normal);
	*norm = s_matrix_to_tuple(&world_normal_matrix);
	norm->w = 0;
	s_vec_norm(norm);
}

