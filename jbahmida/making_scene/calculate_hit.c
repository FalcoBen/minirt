/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_hit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 00:58:15 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 00:58:16 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static bool	is_valid_intersection(t_inters *intersection)
{
	if (intersection->type == T_SPHERE && intersection->sphere)
		return (true);
	else if (intersection->type == T_CYLINDRE && intersection->cylinder)
		return (true);
	else if (intersection->type == T_CONE && intersection->cone)
		return (true);
	else if (intersection->type == T_PLAN && intersection->plane)
		return (true);
	return (false);
}

static void	process_intersection(t_inters *intersection, t_inters *best_hit,
	float *min_t, bool *found)
{
	float	t;

	t = intersection->inters_value;
	if (t < 0)
		return ;
	if (!is_valid_intersection(intersection))
		return ;
	if (t < *min_t)
	{
		*min_t = t;
		*best_hit = *intersection;
		best_hit->existence = true;
		*found = true;
	}
}

static void	find_closest_intersection(t_stack_intersections *inters,
	t_inters *best_hit)
{
	float	min_t;
	bool	found;
	int		i;

	min_t = INFINITY;
	found = false;
	i = 0;
	while (i < inters->count)
	{
		process_intersection(&inters->world_inters_list[i],
			best_hit, &min_t, &found);
		i++;
	}
	if (!found)
		best_hit->existence = false;
}

t_inters	s_hit(t_stack_intersections inters)
{
	t_inters	local;

	local = (t_inters){T_SPHERE, NULL, NULL, NULL, NULL, -1, false};
	if (!inters.existence || inters.count < 1)
		return (local);
	find_closest_intersection(&inters, &local);
	return (local);
}
