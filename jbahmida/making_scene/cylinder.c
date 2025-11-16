/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 03:48:55 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 03:48:56 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	cylinder(t_cylinder **cylinder, t_matrix *trans, bool flag)
{
	static int	id;

	if (!cylinder)
		return ;
	if (flag)
	{
		(*cylinder)->transform = trans;
		(*cylinder)->inv = matrix_inverte((*cylinder)->transform);
		return ;
	}
	id = 0;
	(*cylinder) = alloc(sizeof(t_cylinder), false);
	(*cylinder)->transform = identity_matrix(4, 4);
	(*cylinder)->inv = identity_matrix(4, 4);
	(*cylinder)->minimum = -INFINITY;
	(*cylinder)->maximum = INFINITY;
	(*cylinder)->id = id;
	(*cylinder)->material = material();
	(*cylinder)->closed = false;
	id++;
}

bool	check_cap_cylinder(t_stack_ray *ray, float t)
{
	float	x;
	float	z;

	x = ray->point.x + t * ray->vect.x;
	z = ray->point.z + t * ray->vect.z;
	return ((powf(x, 2) + powf(z, 2)) <= 1);
}

static void	set_cap_value(t_stack_intersections *xs,
	t_cylinder *cylinder, float t)
{
	xs->inters_list[xs->count].inters_value = t;
	xs->inters_list[xs->count].sphere = NULL;
	xs->inters_list[xs->count].plane = NULL;
	xs->inters_list[xs->count].cylinder = cylinder;
	xs->inters_list[xs->count].cone = NULL;
	xs->inters_list[xs->count].existence = true;
	xs->inters_list[xs->count].type = T_CYLINDRE;
	xs->count++;
}

void	intersect_caps_cylinder(t_cylinder *cylinder,
	t_stack_ray *ray, t_stack_intersections *xs)
{
	float	t;

	if (!cylinder->closed || ldbl_cmp(ray->vect.y, 0.0))
		return ;
	t = (cylinder->minimum - ray->point.y) / ray->vect.y;
	if (check_cap_cylinder(ray, t))
		set_cap_value(xs, cylinder, t);
	t = (cylinder->maximum - ray->point.y) / ray->vect.y;
	if (check_cap_cylinder(ray, t))
		set_cap_value(xs, cylinder, t);
	return ;
}

void	s_cylinder_normal(t_tuple *norm, t_cylinder *cylinder, t_tuple *point)
{
	t_nor_cy_nor	var;

	var.point_matrix = s_matrix_tuple(cylinder->inv, point);
	var.local_point = s_matrix_to_tuple(&var.point_matrix);
	var.dist = powf(var.local_point.x, 2) + powf(var.local_point.z, 2);
	if (cylinder->closed && var.dist < 1
		&& var.local_point.y >= cylinder->maximum - FLT_EPSILON)
		var.local_normal = (t_tuple){0, 1, 0, 0};
	else if (cylinder->closed && var.dist < 1
		&& var.local_point.y <= cylinder->minimum + FLT_EPSILON)
		var.local_normal = (t_tuple){0, -1, 0, 0};
	else
		var.local_normal = (t_tuple){var.local_point.x,
			0, var.local_point.z, 0};
	var.transpose = s_matrix_transpos(cylinder->inv);
	var.world_normal_matrix = s_matrix_tuple(&var.transpose, &var.local_normal);
	*norm = s_matrix_to_tuple(&var.world_normal_matrix);
	norm->w = 0;
	s_vec_norm(norm);
}
