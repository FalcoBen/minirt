/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 04:31:30 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 04:31:35 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static void	add_cone_intersection(t_stack_intersections *xs,
		t_cone *cone, float t_value)
{
	xs->inters_list[xs->count].inters_value = t_value;
	xs->inters_list[xs->count].sphere = NULL;
	xs->inters_list[xs->count].cylinder = NULL;
	xs->inters_list[xs->count].plane = NULL;
	xs->inters_list[xs->count].cone = cone;
	xs->inters_list[xs->count].existence = true;
	xs->inters_list[xs->count].type = T_CONE;
	xs->count++;
}

static void	handle_single_root_case(t_nor_cone_inter *var, t_cone *cone,
		t_stack_intersections *xs)
{
	if (!ldbl_cmp(var->b, 0.0))
	{
		var->t = -var->c / (2 * var->b);
		var->y = var->local_ray.point.y + var->t * var->local_ray.vect.y;
		if (var->y > cone->minimum && var->y < cone->maximum)
		{
			add_cone_intersection(xs, cone, var->t);
		}
	}
}

static void	handle_double_root_case(t_nor_cone_inter *var, t_cone *cone,
		t_stack_intersections *xs)
{
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
		var->y0 = var->local_ray.point.y + var->t0 * var->local_ray.vect.y;
		if (var->y0 > cone->minimum && var->y0 < cone->maximum)
		{
			add_cone_intersection(xs, cone, var->t0);
		}
		var->y1 = var->local_ray.point.y + var->t1 * var->local_ray.vect.y;
		if (var->y1 > cone->minimum && var->y1 < cone->maximum)
		{
			add_cone_intersection(xs, cone, var->t1);
		}
	}
}

static void	calculate_cone_coefficients(t_nor_cone_inter *var)
{
	var->a = powf(var->local_ray.vect.x, 2)
		- powf(var->local_ray.vect.y, 2) + powf(var->local_ray.vect.z, 2);
	var->b = 2 * var->local_ray.point.x * var->local_ray.vect.x
		- 2 * var->local_ray.point.y * var->local_ray.vect.y
		+ 2 * var->local_ray.point.z * var->local_ray.vect.z;
	var->c = powf(var->local_ray.point.x, 2)
		- powf(var->local_ray.point.y, 2) + powf(var->local_ray.point.z, 2);
}

// void	cone_intersct(t_stack_intersections *xs, t_cone *cone, t_stack_ray *ray)
// {
// 	t_nor_cone_inter	var;

// 	var.local_ray = s_transform(ray, cone->inv);
// 	calculate_cone_coefficients(&var);
// 	xs->count = 0;
// 	xs->existence = false;
// 	if (ldbl_cmp(var.a, 0.0))
// 	{
// 		handle_single_root_case(&var, cone, xs);
// 	}
// 	else
// 	{
// 		handle_double_root_case(&var, cone, xs);
// 	}
// 	intersect_caps_cone(cone, &var.local_ray, xs);
// 	xs->existence = xs->count > 0;
// 	if (!xs->existence)
// 		xs->world_inters_list = NULL;
// }


void	cone_intersct(t_stack_intersections *xs, t_cone *cone, t_stack_ray *ray)
{
	t_nor_cone_inter	var;

	var.local_ray = s_transform(ray, cone->inv);
	calculate_cone_coefficients(&var);
	xs->count = 0;
	xs->existence = false;
	
	// printf("\n=== CONE INTERSECTION DEBUG ===\n");
	// printf("Cone closed flag: %d\n", cone->closed);
	// printf("Cone min: %f, max: %f\n", cone->minimum, cone->maximum);
	// printf("Local ray origin: (%f, %f, %f)\n", 
	// 	var.local_ray.point.x, var.local_ray.point.y, var.local_ray.point.z);
	// printf("Local ray direction: (%f, %f, %f)\n", 
	// 	var.local_ray.vect.x, var.local_ray.vect.y, var.local_ray.vect.z);
	
	if (ldbl_cmp(var.a, 0.0))
	{
		handle_single_root_case(&var, cone, xs);
	}
	else
	{
		handle_double_root_case(&var, cone, xs);
	}
	
	// printf("Body intersections found: %d\n", xs->count);
	
	intersect_caps_cone(cone, &var.local_ray, xs);
	
	// printf("Total intersections after caps: %d\n", xs->count);
	// printf("=== END DEBUG ===\n\n");
	
	xs->existence = xs->count > 0;
	if (!xs->existence)
		xs->world_inters_list = NULL;
}