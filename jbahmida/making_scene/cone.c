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

t_matrix	*create_cone_transform(t_tuple *position, t_tuple *axis,
	ld radius, ld height)
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

bool	check_cap_cone(t_stack_ray *ray, ld t, ld y)
{
	ld	x;
	ld	z;

	x = ray->point.x + t * ray->vect.x;
	z = ray->point.z + t * ray->vect.z;
	return ((powf(x, 2) + powf(z, 2)) <= fabsf(y));
}

static void	set_cap_value(t_stack_intersections *xs,
	t_cone *cone, ld t)
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
	ld	t;

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
