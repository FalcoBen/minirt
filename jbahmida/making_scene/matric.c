/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matric.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:24:32 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:24:38 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

float	**grid(int row, int col, float *arr)
{
	float	**grid;
	int		i;
	int		j;

	grid = alloc((sizeof(float *) * row), false);
	i = 0;
	while (i < row)
	{
		grid[i] = alloc(sizeof(float) * col, false);
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
	return (grid);
}

t_matrix	*matrix_constractor(int row, int col, float *arr, size_t arr_s)
{
	t_matrix	*matrix;

	if (arr && arr_s != (size_t)(row * col))
		return (NULL);
	matrix = alloc(sizeof(t_matrix), false);
	matrix->row = row;
	matrix->col = col;
	matrix->matrix = grid(row, col, arr);
	return (matrix);
}

static void	initialize_identity_matrix(t_matrix *id, int size)
{
	int	tag;
	int	i;
	int	j;

	tag = 0;
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			if (i == tag && j == tag)
			{
				id->matrix[i][j] = 1;
				tag++;
			}
			j++;
		}
		i++;
	}
}

t_matrix	*identity_matrix(int row, int col)
{
	t_matrix	*id;

	if (row != col)
		return (NULL);
	id = matrix_constractor(row, col, NULL, (size_t) NULL);
	initialize_identity_matrix(id, row);
	return (id);
}

t_matrix	*matrix_transpos(t_matrix *m)
{
	t_matrix	*trans;
	int			i;
	int			j;

	if (!m)
		return (NULL);
	trans = matrix_constractor(m->row, m->col, NULL,
			(size_t) NULL);
	i = 0;
	while (i < m->row)
	{
		j = 0;
		while (j < m->col)
		{
			trans->matrix[j][i] = m->matrix[i][j];
			j++;
		}
		i++;
	}
	copy_matrix_contant(trans);
	return (trans);
}
