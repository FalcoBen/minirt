/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:03:33 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/13 22:03:47 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	cone(t_cone **cone, t_matrix *trans, bool flag)
{
	static int	id;

	id = 0;
	if (!cone)
		return ;
	if (flag)
	{
		(*cone)->transform = trans;
		(*cone)->inv = matrix_inverte((*cone)->transform);
		return ;
	}
	(*cone) = alloc(sizeof(t_cone), false);
	(*cone)->transform = identity_matrix(4, 4);
	(*cone)->inv = identity_matrix(4, 4);
	(*cone)->id = id;
	(*cone)->material = material();
	id++;
}

bool	check_cap_cone(t_stack_ray *ray, float t, float y)
{
	float	x;
	float	z;
	float	radius;

	x = ray->point.x + t * ray->vect.x;
	z = ray->point.z + t * ray->vect.z;
	radius = fabsf(y);
	return ((powf(x, 2) + powf(z, 2)) <= powf(radius, 2));
}

static void	set_cap_value(t_stack_intersections *xs,
	t_cone *cone, float t)
{
	xs->inters_list[xs->count].inters_value = t;
	xs->inters_list[xs->count].sphere = NULL;
	xs->inters_list[xs->count].plane = NULL;
	xs->inters_list[xs->count].cylinder = NULL;
	xs->inters_list[xs->count].cone = cone;
	xs->inters_list[xs->count].existence = true;
	xs->inters_list[xs->count].type = T_CONE;
	xs->count++;
	return ;
}

void	intersect_caps_cone(t_cone *cone,
	t_stack_ray *ray, t_stack_intersections *xs)
{
	float	t;
	float	x;
	float	z;
	float	radius;
	float	dist;

	if (!cone->closed || ldbl_cmp(ray->vect.y, 0.0))
		return ;
	t = (cone->minimum - ray->point.y) / ray->vect.y;
	if (check_cap_cone(ray, t, cone->minimum))
		set_cap_value(xs, cone, t);
	t = (cone->maximum - ray->point.y) / ray->vect.y;
	if (check_cap_cone(ray, t, cone->maximum))
		set_cap_value(xs, cone, t);
	return ;
}
