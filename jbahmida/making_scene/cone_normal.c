/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 04:30:09 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 04:30:13 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static t_tuple	calculate_local_normal(t_cone *cone, t_tuple *local_point)
{
	t_tuple	local_normal;
	ld		dist;
	ld		y;

	dist = powf(local_point->x, 2) + powf(local_point->z, 2);
	if (cone->closed && dist < fabsf(cone->maximum)
		&& local_point->y >= cone->maximum - FLT_EPSILON)
		local_normal = (t_tuple){0, 1, 0, 0};
	else if (cone->closed && dist < fabsf(cone->minimum)
		&& local_point->y <= cone->minimum + FLT_EPSILON)
		local_normal = (t_tuple){0, -1, 0, 0};
	else
	{
		y = sqrtl(powf(local_point->x, 2) + powf(local_point->z, 2));
		if (local_point->y > 0)
			y = -y;
		local_normal.x = local_point->x;
		local_normal.y = y;
		local_normal.z = local_point->z;
		local_normal.w = 0;
	}
	return (local_normal);
}

static t_tuple	transform_to_world_space(t_cone *cone, t_tuple *local_normal)
{
	t_matrix	transpose;
	t_matrix	world_normal;
	t_tuple		norm;

	transpose = s_matrix_transpos(cone->inv);
	world_normal = s_matrix_tuple(&transpose, local_normal);
	norm = s_matrix_to_tuple(&world_normal);
	norm.w = 0;
	return (norm);
}

static t_tuple	get_local_point(t_cone *cone, t_tuple *point)
{
	t_matrix	point_matrix;
	t_tuple		local_point;

	point_matrix = s_matrix_tuple(cone->inv, point);
	local_point = s_matrix_to_tuple(&point_matrix);
	return (local_point);
}

void	s_cone_normal(t_tuple *norm, t_cone *cone, t_tuple *point)
{
	t_tuple	local_point;
	t_tuple	local_normal;
	t_tuple	world_normal;

	local_point = get_local_point(cone, point);
	local_normal = calculate_local_normal(cone, &local_point);
	world_normal = transform_to_world_space(cone, &local_normal);
	*norm = world_normal;
	s_vec_norm(norm);
}
