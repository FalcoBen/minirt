/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 21:15:10 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 21:15:14 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_material	*material(void)
{
	t_material	*m;

	m = alloc(sizeof(t_material), false);
	m->color = alloc(sizeof(t_color), false);
	return (m);
}

t_stack_ray	s_ray_constrctor(t_tuple *origin, t_tuple *vect)
{
	t_stack_ray	ray;

	ray = (t_stack_ray){(t_tuple){0, 0, 0, 1},
		(t_tuple){0, 0, 0, 0}};
	if (!origin || !vect)
		return (ray);
	ray.point = *origin;
	ray.vect = *vect;
	return (ray);
}

t_stack_ray	s_transform(t_stack_ray *ray, t_matrix *m)
{
	t_stack_ray	local;
	t_matrix	m1;
	t_matrix	m2;
	t_tuple		point;
	t_tuple		vect;

	local = (t_stack_ray){(t_tuple){0, 0, 0, 1},
		(t_tuple){0, 0, 0, 0}};
	if (!ray || !m)
		return (local);
	m1 = s_matrix_tuple(m, &ray->point);
	point = s_matrix_to_tuple1(&m1);
	m2 = s_matrix_tuple(m, &ray->vect);
	vect = s_matrix_to_tuple1(&m2);
	local.point = point;
	local.vect = vect;
	return (local);
}

void	s_grid(int row, int col, float *arr, float grid[4][4])
{
	int	i;
	int	j;

	i = 0;
	while (i < row)
	{
		j = 0;
		while (j < col)
		{
			if (!arr)
				grid[i][j] = 0;
			else
				grid[i][j] = arr[i * col + j];
			j++;
		}
		i++;
	}
	return ;
}

t_matrix	s_matrix_constractor(int row, int col, float *arr)
{
	t_matrix	matrix;

	matrix.col = -1;
	matrix.row = -1;
	matrix.matrix = NULL;
	matrix.row = row;
	matrix.col = col;
	matrix.matrix = NULL;
	s_grid(row, col, arr, matrix.stack_matrix);
	return (matrix);
}
