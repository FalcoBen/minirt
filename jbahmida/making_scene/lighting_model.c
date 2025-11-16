/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting_model.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 00:34:31 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 00:34:33 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static t_color	calculate_ambient(t_color *effective_color,
	t_material *material)
{
	return (s_color_scalar(effective_color, material->ambient));
}

static t_color	calculate_diffuse(t_nor_s_lighting *local)
{
	ld	light_dot_normal;

	light_dot_normal = dot_product(&local->lightv, &local->normalv);
	if (light_dot_normal >= 0)
	{
		return (s_color_scalar(&local->effective_color,
				local->material.diffuse * light_dot_normal));
	}
	return ((t_color){0, 0, 0});
}

static t_color	calculate_specular(t_nor_s_lighting *local)
{
	t_tuple	neg_lightv;
	t_tuple	reflectv;
	ld		reflect_dot_eye;
	ld		factor;

	neg_lightv = s_neg_t(local->lightv);
	reflectv = s_reflect(neg_lightv, local->normalv);
	s_vec_norm(&reflectv);
	s_vec_norm(&local->eyev);
	reflect_dot_eye = dot_product(&reflectv, &local->eyev);
	if (reflect_dot_eye > 0)
	{
		factor = pow(reflect_dot_eye, local->material.shininess);
		return (s_color_scalar(local->light.intensity,
				local->material.specular * factor));
	}
	return ((t_color){0, 0, 0});
}

static void	get_light_vector(t_nor_s_lighting *local, t_tuple *position)
{
	local->lightv = (t_tuple){0, 0, 0, 0};
	s_sub_t(&local->lightv, *(local->light.position), *position);
	s_vec_norm(&local->lightv);
}

t_color	s_lighting(t_world *world, t_comp *comp, t_nor_shade_hit var)
{
	t_nor_s_lighting	local;

	local.material = var.temp_material;
	local.light = *(world->lights[var.i]);
	local.effective_color = s_hadamard(local.material.color,
			local.light.intensity);
	local.normalv = comp->normalv;
	local.eyev = comp->eyev;
	if (var.shadowed)
		return (calculate_ambient(&local.effective_color, &local.material));
	get_light_vector(&local, &comp->over_point);
	local.ambient = calculate_ambient(&local.effective_color, &local.material);
	local.diffuse = calculate_diffuse(&local);
	local.specular = calculate_specular(&local);
	local.tmp = s_color_add(&local.ambient, &local.diffuse);
	return (s_color_add(&local.tmp, &local.specular));
}
