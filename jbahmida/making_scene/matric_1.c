/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matric_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:31:42 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:31:47 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

ld	det_small_matrix(t_matrix *m)
{
	ld	det;
	ld	**ptr;

	if (!m || m->row != 2 || m->col != 2)
		return (0);
	ptr = m->matrix;
	det = (ptr[0][0] * ptr[1][1]) - (ptr[1][0] * ptr[0][1]);
	return (det);
}

static int	calculate_submatrix_size(t_matrix *m)
{
	return ((m->row - 1) * (m->col - 1));
}

static void	copy_submatrix_elements(t_submatrix_data *data)
{
	int	v;
	int	i;
	int	j;

	v = 0;
	i = 0;
	while (i < data->m->row && v < data->va_size)
	{
		if (i != data->r)
		{
			j = 0;
			while (j < data->m->col && v < data->va_size)
			{
				if (j != data->c)
					data->va_list[v++] = data->m->matrix[i][j];
				j++;
			}
		}
		i++;
	}
}

static bool	is_valid_submatrix_request(t_matrix *m, int va_size)
{
	if (!m)
		return (false);
	if (va_size <= 0)
		return (false);
	return (true);
}

t_matrix	*submatrix(t_matrix *m, int r, int c)
{
	t_matrix			*sub;
	ld					*va_list;
	int					va_size;
	t_submatrix_data	data;

	va_size = calculate_submatrix_size(m);
	if (!is_valid_submatrix_request(m, va_size))
		return (NULL);
	va_list = alloc(sizeof(ld) * va_size, false);
	data.m = m;
	data.r = r;
	data.c = c;
	data.va_list = va_list;
	data.va_size = va_size;
	copy_submatrix_elements(&data);
	sub = matrix_constractor(m->row - 1, m->col - 1, va_list, va_size);
	return (sub);
}
