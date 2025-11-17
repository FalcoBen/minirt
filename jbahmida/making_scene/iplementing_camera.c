/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iplementing_camera.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 22:12:53 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/12 22:12:58 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_stack_ray	s_ray_for_camera(t_camera *cam, float x, float y)
{
	t_stack_ray				ray;
	t_nor_ray_for_camera	var;

	var.xoffset = (x + 0.5) * cam->pixel_size;
	var.yoffset = (y + 0.5) * cam->pixel_size;
	var.world_x = cam->half_width - var.xoffset;
	var.world_y = cam->half_height - var.yoffset;
	var.t = s_create_tuple((float [3]){var.world_x, var.world_y, -1}, 1);
	var.tmp = s_matrix_tuple(cam->tran_inv, &var.t);
	var.pixel = s_matrix_to_tuple(&var.tmp);
	var.t1 = s_create_tuple((float [3]){0, 0, 0}, 1);
	var.tmp1 = s_matrix_tuple(cam->tran_inv, &var.t1);
	var.origin = s_matrix_to_tuple(&var.tmp1);
	s_sub_t(&var.direction, var.pixel, var.origin);
	s_vec_norm(&var.direction);
	ray = s_ray_constrctor(&var.origin, &var.direction);
	return (ray);
}

static void	camera_helper(t_camera *new, t_matrix *m)
{
	if (!m)
		new->tran = identity_matrix(4, 4);
	else
		new->tran = m;
	new->tran_inv = matrix_inverte(new->tran);
	return ;
}

t_camera	*camera(float hsize, float vsize, float field_ov, t_matrix *m)
{
	t_camera	*new;
	float		half_view;

	half_view = tanf(field_ov / 2);
	new = alloc(sizeof(t_camera), false);
	new->hsize = hsize;
	new->vsize = vsize;
	new->field_ov = field_ov;
	camera_helper(new, m);
	new->aspect = hsize / vsize;
	if (new->aspect >= 1)
	{
		new->half_width = half_view;
		new->half_height = half_view / new->aspect;
	}
	else
	{
		new->half_width = new->aspect * half_view;
		new->half_height = half_view;
	}
	new->pixel_size = (new->half_width * 2) / hsize;
	copy_matrix_contant(new->tran);
	copy_matrix_contant(new->tran_inv);
	return (new);
}
