/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_computations_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 02:00:15 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 02:00:30 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_material	*get_material(t_comp *comp)
{
	if (!comp)
		return (NULL);
	if (comp->obj_type == T_SPHERE)
		return (((t_sphere *)comp->obj)->material);
	else if (comp->obj_type == T_CYLINDRE)
		return (((t_cylinder *)comp->obj)->material);
	else if (comp->obj_type == T_CONE)
		return (((t_cone *)comp->obj)->material);
	else if (comp->obj_type == T_PLAN)
		return (((t_plane *)comp->obj)->material);
	return (NULL);
}

static void	toning_color(t_color *final_color)
{
	if (final_color->r > 1.0)
		final_color->r = 1.0;
	else if (final_color->r < 0.0)
		final_color->r = 0.0;
	else
		final_color->r = final_color->r;
	if (final_color->g > 1.0)
		final_color->g = 1.0;
	else if (final_color->g < 0.0)
		final_color->g = 0.0;
	else
		final_color->g = final_color->g;
	if (final_color->b > 1.0)
		final_color->b = 1.0;
	else if (final_color->b < 0.0)
		final_color->b = 0.0;
	else
		final_color->b = final_color->b;
}

t_color	shade_hit(t_world *world, t_comp *comp)
{
	t_nor_shade_hit	var;

	var.final_color = (t_color){0, 0, 0};
	var.material = get_material(comp);
	if (!material || !world || !comp)
		return (var.final_color);
	var.temp_material = *var.material;
	var.base_color = shade_helper(var.material, comp);
	var.temp_material.color = &var.base_color;
	var.i = 0;
	while (var.i < world->light_num)
	{
		var.shadowed = is_shadowed(world,
				comp->over_point, world->lights[var.i]);
		var.light_contribution = s_lighting(world, comp, var);
		var.final_color.r += var.light_contribution.r;
		var.final_color.g += var.light_contribution.g;
		var.final_color.b += var.light_contribution.b;
		var.i++;
	}
	toning_color(&var.final_color);
	return (var.final_color);
}

t_tuple	s_position(t_stack_ray *ray, float t)
{
	t_tuple	pos;
	t_tuple	scalar_res;

	s_scalar_mult(&scalar_res, ray->vect, t);
	s_add_t(&pos, ray->point, scalar_res);
	return (pos);
}
