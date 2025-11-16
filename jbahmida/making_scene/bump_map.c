/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:48 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/16 02:46:50 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	sphere_uv(t_tuple *point, t_sphere *sphere, float *u, float *v)
{
	t_matrix	point_matrix;
	t_tuple		local_point;
	float		phi;
	float		theta;

	point_matrix = s_matrix_tuple(sphere->inv, point);
	local_point = s_matrix_to_tuple(&point_matrix);
	phi = atan2(local_point.z, local_point.x);
	theta = asin(local_point.y);
	*u = 1 - (phi + M_PI) / (2 * M_PI);
	*v = (theta + M_PI / 2) / M_PI;
	if (sphere->material->has_bump_map
		&& sphere->material->bump_map
		&& sphere->material->bump_map->type == 1)
	{
		*u *= 2.0;
		*v *= 2.0;
	}
}

void	cone_uv(t_tuple *point, t_cone *cone, float *u, float *v)
{
	t_matrix	point_matrix;
	t_tuple		local_point;
	float		theta;
	float		range;

	point_matrix = s_matrix_tuple(cone->inv, point);
	local_point = s_matrix_to_tuple(&point_matrix);
	theta = atan2(local_point.x, local_point.z);
	*u = 1.0 - (theta + M_PI) / (2.0 * M_PI);
	range = cone->maximum - cone->minimum;
	if (fabs(range) > 0.0001)
		*v = (local_point.y - cone->minimum) / range;
	else
		*v = 0.5;
	*u = fmod(*u, 1.0);
	if (*u < 0)
		*u += 1.0;
	*v = fmax(0.0, fmin(1.0, *v));
}

void	plane_uv(t_tuple *point, t_plane *plane, float *u, float *v)
{
	if (plane->material->has_color_texture && \
			plane->material->color_texture && \
			plane->material->color_texture->type == 1)
	{
		plane_uv_checker(point, plane, u, v);
	}
	else if (plane->material->has_bump_map && \
			plane->material->bump_map && \
			plane->material->bump_map->type == 2 && \
			plane->material->bump_map->image)
	{
		plane_uv_image(point, plane, u, v);
	}
	else
	{
		*u = 0.0;
		*v = 0.0;
	}
}

t_color	shade_helper(t_material *material, t_comp *comp)
{
	t_color			base_color;
	float			u;
	float			v;
	t_texture_color	tex_color;

	base_color = *(material->color);
	get_uv_coors(comp, &u, &v);
	if (material->has_color_texture && material->color_texture
		&& material->color_texture->type == 1)
	{
		tex_color = sample_color(material->color_texture, u, v);
		base_color.r = tex_color.x;
		base_color.g = tex_color.y;
		base_color.b = tex_color.z;
	}
	else if (material->has_bump_map && material->bump_map
		&& material->bump_map->type == 2
		&& material->bump_map->image)
	{
		tex_color = sample_color(material->bump_map, u, v);
		base_color.r = tex_color.x;
		base_color.g = tex_color.y;
		base_color.b = tex_color.z;
	}
	return (base_color);
}
