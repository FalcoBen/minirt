/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 04:42:08 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/12 04:42:13 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	copy_matrix_contant(t_matrix *m)
{
	int	i;
	int	j;

	if (!m || !m->matrix)
		return ;
	i = 0;
	while (i < m->row)
	{
		j = 0;
		while (j < m->col)
		{
			m->stack_matrix[i][j] = m->matrix[i][j];
			j++;
		}
		i++;
	}
	return ;
}

t_color	s_create_color(ld *vals)
{
	t_color	color;

	color = (t_color){0, 0, 0};
	color.r = vals[0];
	color.g = vals[1];
	color.b = vals[2];
	return (color);
}

t_color	color_at(t_world *w, t_stack_ray *ray)
{
	t_color					res;
	t_stack_intersections	xs;
	t_inters				h;
	t_comp					comp;

	res = (t_color){0, 0, 0};
	s_intersect_world(&xs, *w, *ray);
	h = s_hit(xs);
	free(xs.world_inters_list);
	if (h.existence)
	{
		comp = prepare_computations(&h, ray);
		return (shade_hit(w, &comp));
	}
	else
		return (res);
}

t_canva	s_canva_create(mlx_t *p, int w, int h)
{
	t_canva	local;

	local = (t_canva){NULL, NULL, NULL, NULL};
	local.mlx = p;
	local.image = mlx_new_image(p, w, h);
	return (local);
}

int	combine_color(t_color color)
{
	int	c;

	if (color.r > 1)
		color.r = 1;
	else if (color.r < 0)
		color.r = 0;
	if (color.g > 1)
		color.g = 1;
	else if (color.g < 0)
		color.g = 0;
	if (color.b > 1)
		color.b = 1;
	else if (color.b < 0)
		color.b = 0;
	c = ((int)(color.b * 255) << 16)
		| ((int)(color.g * 255) << 8)
		| ((int)(color.r * 255)) | (255 << 24);
	return (c);
}
