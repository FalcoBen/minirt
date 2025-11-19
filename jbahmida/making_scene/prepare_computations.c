/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_computations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:47:02 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 00:47:03 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	compute_normal_at(t_tuple *normal,
	t_inters *intersection, t_tuple *point)
{
	if (intersection->sphere != NULL)
		s_normal_at(normal, intersection->sphere, point);
	else if (intersection->cylinder != NULL)
		s_cylinder_normal(normal, intersection->cylinder, point);
	else if (intersection->cone != NULL)
		s_cone_normal(normal, intersection->cone, point);
	else if (intersection->plane != NULL)
		s_plane_normal(normal, intersection->plane, point);
}

static void	decide_object(t_comp *new, t_inters *intersection)
{
	if (intersection->sphere != NULL)
	{
		new->obj = intersection->sphere;
		new->obj_type = T_SPHERE;
	}
	else if (intersection->cylinder != NULL)
	{
		new->obj = intersection->cylinder;
		new->obj_type = T_CYLINDRE;
	}
	else if (intersection->cone != NULL)
	{
		new->obj = intersection->cone;
		new->obj_type = T_CONE;
	}
	else if (intersection->plane != NULL)
	{
		new->obj = intersection->plane;
		new->obj_type = T_PLAN;
	}
	new->intersection = intersection;
	new->t = intersection->inters_value;
	return ;
}

static void	p_c_normalev(t_comp *new)
{
	t_tuple	normalv;

	compute_normal_at(&normalv, new->intersection, &new->point);
	s_vec_norm(&normalv);
	if (dot_product(&normalv, &new->eyev) < 0)
	{
		new->inside = true;
		new->normalv = s_neg_t(normalv);
	}
	else
	{
		new->inside = false;
		new->normalv = normalv;
	}
	return ;
}

t_comp	prepare_computations(t_inters *intersection, t_stack_ray *ray)
{
	t_comp	new;
	t_tuple	scaled_normal;

	new = (t_comp){NULL, T_SPHERE, {0, 0, 0, 0}, {0, 0, 0, 0},
	{0, 0, 0, 0}, {0, 0, 0, 0}, NULL, 0, false};
	if (!intersection || !ray)
		return (new);
	decide_object(&new, intersection);
	new.point = s_position(ray, intersection->inters_value);
	new.eyev = s_neg_t(ray->vect);
	p_c_normalev(&new);
	s_scalar_mult(&scaled_normal, new.normalv, SHADOW_EPSILON);
	s_add_t(&new.over_point, new.point, scaled_normal);
	return (new);
}
