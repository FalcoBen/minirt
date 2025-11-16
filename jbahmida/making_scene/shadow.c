/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 23:18:09 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/12 23:18:21 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static bool	is_shadowed_help(t_stack_intersections	xs, float distance)
{
	t_inters	h;
	bool		shadowed;

	h = s_hit(xs);
	shadowed = false;
	if (h.existence
		&& h.inters_value > SHADOW_EPSILON
		&& h.inters_value < (distance - SHADOW_EPSILON))
	{
		shadowed = true;
	}
	if (xs.world_inters_list)
		free(xs.world_inters_list);
	return (shadowed);
}

bool	is_shadowed(t_world *world, t_tuple point, t_light *light)
{
	t_tuple					light_direction;
	float					distance;
	t_stack_ray				shadow_ray;
	t_stack_intersections	xs;

	s_sub_t(&light_direction, *(light->position), point);
	distance = magn(&light_direction);
	s_vec_norm(&light_direction);
	shadow_ray.point = point;
	shadow_ray.vect = light_direction;
	xs.count = 0;
	xs.existence = false;
	xs.world_inters_list = NULL;
	s_intersect_world(&xs, *world, shadow_ray);
	if (!xs.existence || xs.count == 0)
	{
		if (xs.world_inters_list)
			free(xs.world_inters_list);
		return (false);
	}
	return (is_shadowed_help(xs, distance));
}
