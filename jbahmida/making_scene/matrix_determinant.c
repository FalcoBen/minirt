/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_determinant.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:41:33 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:41:34 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static float	**allocate_matrix_from_stack(t_matrix *m)
{
	float	**ptr;
	int		i;
	int		j;

	ptr = alloc((sizeof(float *) * m->row), false);
	i = 0;
	while (i < m->row)
	{
		ptr[i] = alloc((sizeof(float) * m->col), false);
		j = 0;
		while (j < m->col)
		{
			ptr[i][j] = m->stack_matrix[i][j];
			j++;
		}
		i++;
	}
	return (ptr);
}

static void	free_allocated_matrix(float **ptr, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

static float	calculate_large_determinant(t_matrix *m, float **ptr)
{
	float	det;
	int		i;

	det = 0;
	i = 0;
	while (i < m->col)
	{
		det += ptr[0][i] * matrix_cofactor(m, 0, i);
		i++;
	}
	return (det);
}

static void	setup_matrix_pointer(t_matrix *m, float ***ptr_ptr)
{
	if (m->matrix)
		*ptr_ptr = m->matrix;
	else
		*ptr_ptr = allocate_matrix_from_stack(m);
}

float	matrix_determinant(t_matrix *m)
{
	float	**ptr;
	float	det;

	if (!m)
		return (EIO);
	setup_matrix_pointer(m, &ptr);
	if (m->row == 2 && m->col == 2)
		det = det_small_matrix(m);
	else
		det = calculate_large_determinant(m, ptr);
	return (det);
}
