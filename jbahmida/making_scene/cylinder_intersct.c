/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersct.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 03:01:59 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 03:02:03 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	handle_cylinder_body_intersection(t_nor_cy_inter *var,
		t_cylinder *cylinder, t_stack_intersections *xs)
{
	var->y0 = var->local_ray.point.y + var->t0 * var->local_ray.vect.y;
	if (var->y0 > cylinder->minimum && var->y0 < cylinder->maximum)
	{
		xs->inters_list[xs->count].inters_value = var->t0;
		xs->inters_list[xs->count].sphere = NULL;
		xs->inters_list[xs->count].plane = NULL;
		xs->inters_list[xs->count].cylinder = cylinder;
		xs->inters_list[xs->count].cone = NULL;
		xs->inters_list[xs->count].existence = true;
		xs->inters_list[xs->count].type = T_CYLINDRE;
		xs->count++;
	}
	var->y1 = var->local_ray.point.y + var->t1 * var->local_ray.vect.y;
	if (var->y1 > cylinder->minimum && var->y1 < cylinder->maximum)
	{
		xs->inters_list[xs->count].inters_value = var->t1;
		xs->inters_list[xs->count].sphere = NULL;
		xs->inters_list[xs->count].plane = NULL;
		xs->inters_list[xs->count].cylinder = cylinder;
		xs->inters_list[xs->count].cone = NULL;
		xs->inters_list[xs->count].existence = true;
		xs->inters_list[xs->count].type = T_CYLINDRE;
		xs->count++;
	}
}

static void	calculate_delta(t_nor_cy_inter *var,
	t_cylinder *cylinder, t_stack_intersections *xs)
{
	if (!ldbl_cmp(var->a, 0.0))
	{
		var->b = (2 * var->local_ray.point.x * var->local_ray.vect.x)
			+ (2 * var->local_ray.point.z * var->local_ray.vect.z);
		var->c = powf(var->local_ray.point.x, 2)
			+ powf(var->local_ray.point.z, 2) - 1;
		var->delta = powf(var->b, 2) - 4 * var->a * var->c;
		if (var->delta >= 0)
		{
			var->t0 = ((-var->b) - sqrtl(var->delta)) / (2 * var->a);
			var->t1 = ((-var->b) + sqrtl(var->delta)) / (2 * var->a);
			if (var->t0 > var->t1)
			{
				var->tmp = var->t0;
				var->t0 = var->t1;
				var->t1 = var->tmp;
			}
			handle_cylinder_body_intersection(var, cylinder, xs);
		}
	}
}

void	cylinder_intersct(t_stack_intersections *xs,
		t_cylinder *cylinder, t_stack_ray *ray)
{
	t_nor_cy_inter	var;

	var.local_ray = s_transform(ray, cylinder->inv);
	var.a = powf(var.local_ray.vect.x, 2) + powf(var.local_ray.vect.z, 2);
	xs->count = 0;
	xs->existence = false;
	calculate_delta(&var, cylinder, xs);
	intersect_caps_cylinder(cylinder, &var.local_ray, xs);
	xs->existence = xs->count > 0;
	if (!xs->existence)
		xs->world_inters_list = NULL;
}

void	infinit_cylinder_detect(t_tuple *local_point, ld *v)
{
	*v = local_point->y * 0.1;
	*v = fmod(*v, 1.0);
	if (*v < 0)
		*v += 1.0;
}

void	cylinder_uv(t_tuple *point, t_cylinder *cylinder, ld *u, ld *v)
{
	t_matrix	point_matrix;
	t_tuple		local_point;
	ld			theta;
	ld			range;

	point_matrix = s_matrix_tuple(cylinder->inv, point);
	local_point = s_matrix_to_tuple(&point_matrix);
	theta = atan2(local_point.x, local_point.z);
	*u = 1.0 - (theta + M_PI) / (2.0 * M_PI);
	if (isinf(cylinder->minimum) || isinf(cylinder->maximum))
		infinit_cylinder_detect(&local_point, v);
	else
	{
		range = cylinder->maximum - cylinder->minimum;
		if (fabs(range) > 0.0001)
			*v = (local_point.y - cylinder->minimum) / range;
		else
			*v = 0.5;
	}
	*u = fmod(*u, 1.0);
	if (*u < 0)
		*u += 1.0;
	if (isnan(*v) || isinf(*v))
		*v = 0.0;
}
