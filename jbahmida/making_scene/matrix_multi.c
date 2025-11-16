/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_multi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:52:22 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:52:23 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

typedef struct s_matrix_mult_data
{
	t_matrix	*m1;
	t_matrix	*m2;
	ld			*va_list;
	int			v_size;
}	t_matrix_mult_data;

static bool	is_valid_matrix_multiplication(t_matrix *m1, t_matrix *m2)
{
	if (!m1 || !m2 || m1->col != m2->row)
		return (false);
	return (true);
}

static void	calculate_matrix_product(t_matrix_mult_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < data->m1->row)
	{
		j = 0;
		while (j < data->m2->col)
		{
			data->va_list[i * data->m2->col + j] = 0;
			k = 0;
			while (k < data->m1->col)
			{
				data->va_list[i * data->m2->col + j]
					+= data->m1->matrix[i][k] * data->m2->matrix[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
}

t_matrix	*matrix_multi(t_matrix *m1, t_matrix *m2)
{
	t_matrix			*res;
	ld					*va_list;
	int					v_size;
	t_matrix_mult_data	data;

	if (!is_valid_matrix_multiplication(m1, m2))
		return (NULL);
	v_size = m1->row * m2->col;
	va_list = alloc(sizeof(ld) * v_size, false);
	data.m1 = m1;
	data.m2 = m2;
	data.va_list = va_list;
	data.v_size = v_size;
	calculate_matrix_product(&data);
	res = matrix_constractor(m1->row, m2->col, va_list, v_size);
	copy_matrix_contant(res);
	return (res);
}
