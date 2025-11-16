/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_transform.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 04:31:55 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 04:32:00 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static t_matrix	*handle_parallel_vectors(ld c)
{
	if (c > 0)
		return (identity_matrix(4, 4));
	else
		return (rotation_x(M_PI));
}

static void	initialize_vx_matrix(t_tuple *v, ld vx[9])
{
	vx[0] = 0.0;
	vx[1] = -v->z;
	vx[2] = v->y;
	vx[3] = v->z;
	vx[4] = 0.0;
	vx[5] = -v->x;
	vx[6] = -v->y;
	vx[7] = v->x;
	vx[8] = 0.0;
}

static void	calculate_rotation_matrix(t_matrix *result, ld vx[9], ld factor)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			k = 0;
			result->matrix[i][j] += vx[i * 3 + j];
			while (k < 3)
			{
				result->matrix[i][j] += vx[(i * 3) + k]
					* vx[(k * 3) + j] * factor;
				k++;
			}
			j++;
		}
		i++;
	}
}

t_matrix	*rotation_from_to(t_tuple *from, t_tuple *to)
{
	t_nor_roft	var;

	var.v = s_cross_product(from, to);
	var.s = sqrt(var.v.x * var.v.x + var.v.y * var.v.y + var.v.z * var.v.z);
	var.c = dot_product(from, to);
	if (var.s < 0.0001)
		return (handle_parallel_vectors(var.c));
	initialize_vx_matrix(&var.v, var.vx);
	var.result = identity_matrix(4, 4);
	var.factor = (1 - var.c) / (var.s * var.s);
	calculate_rotation_matrix(var.result, var.vx, var.factor);
	return (var.result);
}
