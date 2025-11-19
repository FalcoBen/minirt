/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_normal_extra.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:34:02 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/19 19:34:05 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_tuple	cap_normal_max(t_cone *cone, t_tuple *p, float dist_sq)
{
	float	max_radius_sq;
	float	y_tolerance;
	float	radius_tolerance;

	radius_tolerance = 0.001f;
	y_tolerance = 0.0001f;
	if (cone->closed && p->y >= (cone->maximum - y_tolerance))
	{
		max_radius_sq = powf(fabsf(cone->maximum), 2);
		if (dist_sq <= max_radius_sq + radius_tolerance)
			return ((t_tuple){0, 1, 0, 0});
	}
	return ((t_tuple){0, 0, 0, 0});
}

t_tuple	cap_normal_min(t_cone *cone, t_tuple *p, float dist_sq)
{
	float	min_radius_sq;
	float	y_tolerance;
	float	radius_tolerance;

	radius_tolerance = 0.001f;
	y_tolerance = 0.0001f;
	if (cone->closed && p->y <= (cone->minimum + y_tolerance))
	{
		min_radius_sq = powf(fabsf(cone->minimum), 2);
		if (dist_sq <= min_radius_sq + radius_tolerance)
			return ((t_tuple){0, -1, 0, 0});
	}
	return ((t_tuple){0, 0, 0, 0});
}

t_tuple	side_normal(t_tuple *p, float radius)
{
	t_tuple	n;

	if (p->y >= 0)
	{
		n.x = p->x;
		n.y = -radius;
		n.z = p->z;
	}
	else
	{
		n.x = p->x;
		n.y = radius;
		n.z = p->z;
	}
	n.w = 0;
	return (n);
}
