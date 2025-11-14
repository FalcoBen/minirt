/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 22:52:49 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/11 22:52:54 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	sphere(t_sphere **sphere, t_matrix *tran, bool flag)
{
	static int	id;

	id = 0;
	if (!sphere)
		return ;
	if (flag)
	{
		(*sphere)->transform = tran;
		(*sphere)->inv = matrix_inverte((*sphere)->transform);
		return ;
	}
	(*sphere) = alloc(sizeof(t_sphere), false);
	(*sphere)->transform = identity_matrix(4, 4);
	(*sphere)->inv = identity_matrix(4, 4);
	(*sphere)->origin = tuple_constractor(0, 0, 0, 1);
	(*sphere)->radius = 1;
	(*sphere)->id = id;
	(*sphere)->material = material();
	id++;
}

static void	s_intersect_helper(t_stack_intersections *xs, t_nor_intersect var,
	t_sphere *s)
{
	xs->count = 2;
	var.sqrt_delta = sqrtl(var.delta);
	xs->inters_list[0].inters_value = (-var.b
			- var.sqrt_delta) / (2 * var.a);
	xs->inters_list[1].inters_value = (-var.b
			+ var.sqrt_delta) / (2 * var.a);
	xs->inters_list[0].sphere = s;
	xs->inters_list[1].sphere = s;
	xs->inters_list[0].cylinder = NULL;
	xs->inters_list[1].cylinder = NULL;
	xs->inters_list[0].cone = NULL;
	xs->inters_list[1].cone = NULL;
	xs->inters_list[0].plane = NULL;
	xs->inters_list[1].plane = NULL;
	xs->inters_list[0].existence = true;
	xs->inters_list[1].existence = true;
	xs->inters_list[0].type = T_SPHERE;
	xs->inters_list[1].type = T_SPHERE;
}

static void	calculate_delta(t_nor_intersect	*var)
{
	var->a = dot_product(&var->local_ray.vect, &var->local_ray.vect);
	var->b = 2 * dot_product(&var->local_ray.point, &var->local_ray.vect);
	var->c = dot_product(&var->local_ray.point, &var->local_ray.point) - 1;
	var->delta = pow(var->b, 2) - 4 * var->a * var->c;
}

void	s_intersect(t_stack_intersections *xs, t_stack_ray ray, t_sphere *s)
{
	t_nor_intersect	var;

	var.local_ray = s_transform(&ray, s->inv);
	calculate_delta(&var);
	if (var.delta < 0.00)
	{
		xs->count = 0;
		xs->existence = false;
		xs->world_inters_list = NULL;
		return ;
	}
	xs->existence = true;
	if (var.delta < INTER_EPSILON)
	{
		xs->count = 1;
		xs->inters_list[0].inters_value = (-var.b) / (2 * var.a);
		xs->inters_list[0].sphere = s;
		xs->inters_list[0].cylinder = NULL;
		xs->inters_list[0].cone = NULL;
		xs->inters_list[0].plane = NULL;
		xs->inters_list[0].existence = true;
		xs->inters_list[0].type = T_SPHERE;
	}
	else
		s_intersect_helper(xs, var, s);
}

void	s_normal_at(t_tuple *normal, t_sphere *s, t_tuple *point)
{
	t_matrix	m;
	t_tuple		obj_point;
	t_tuple		obj_normal;
	t_matrix	inv_transpose;
	t_tuple		w_normal;

	copy_matrix_contant(s->inv);
	m = s_matrix_tuple(s->inv, point);
	obj_point = s_matrix_to_tuple(&m);
	obj_normal = (t_tuple){0, 0, 0, 0};
	obj_normal = (t_tuple){0, 0, 0, 0};
	s_sub_t(&obj_normal, obj_point, *(s->origin));
	inv_transpose = s_matrix_transpos(s->inv);
	m = s_matrix_tuple(&inv_transpose, &obj_normal);
	w_normal = s_matrix_to_tuple(&m);
	s_vec_norm(&w_normal);
	normal->x = w_normal.x;
	normal->y = w_normal.y;
	normal->z = w_normal.z;
	normal->w = w_normal.w;
	return ;
}

