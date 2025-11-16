/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_cofactor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:44:30 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:44:31 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static void	allocate_matrix_from_stack_data(t_matrix *m)
{
	int	i;
	int	j;

	m->matrix = alloc(sizeof(float *) * m->row, false);
	i = 0;
	while (i < m->row)
	{
		m->matrix[i] = alloc(sizeof(float) * m->col, false);
		j = 0;
		while (j < m->col)
		{
			m->matrix[i][j] = m->stack_matrix[i][j];
			j++;
		}
		i++;
	}
}

static float	calculate_cofactor_value(int row, int col, float determinant)
{
	if ((row + col) % 2 == 0)
		return (determinant);
	else
		return (-determinant);
}

static float	compute_minor_determinant(t_matrix *m, int row, int col)
{
	t_matrix	*minor;
	float		det;

	minor = submatrix(m, row, col);
	det = matrix_determinant(minor);
	return (det);
}

float	matrix_cofactor(t_matrix *m, int row, int col)
{
	float	determinant;

	if (!m)
		return (EIO);
	if (!m->matrix)
		allocate_matrix_from_stack_data(m);
	determinant = compute_minor_determinant(m, row, col);
	return (calculate_cofactor_value(row, col, determinant));
}
