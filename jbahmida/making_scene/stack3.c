/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 00:56:01 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 00:56:05 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static void	initialize_result_array(t_matrix *m1, t_matrix *m2, ld *va_list)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < m1->row)
	{
		j = 0;
		while (j < m2->col)
		{
			va_list[i * m2->col + j] = 0;
			k = 0;
			while (k < m1->col)
			{
				va_list[i * m2->col + j] += m1->stack_matrix[i][k]
					* m2->stack_matrix[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
}

static t_matrix	create_error_matrix(void)
{
	t_matrix	local;

	local.col = -1;
	local.row = -1;
	return (local);
}

static bool	validate_matrices(t_matrix *m1, t_matrix *m2)
{
	if (!m1 || !m2 || m1->col != m2->row)
		return (false);
	return (true);
}

t_matrix	s_matrix_multi(t_matrix *m1, t_matrix *m2)
{
	t_matrix	local;
	int			v_size;
	ld			*va_list;

	if (m1->matrix)
		copy_matrix_contant(m1);
	if (m2->matrix)
		copy_matrix_contant(m2);
	if (!validate_matrices(m1, m2))
		return (create_error_matrix());
	v_size = m1->row * m2->col;
	va_list = (ld *)malloc(sizeof(ld) * v_size);
	if (!va_list)
		return (create_error_matrix());
	initialize_result_array(m1, m2, va_list);
	local = s_matrix_constractor(m1->row, m2->col, va_list);
	free(va_list);
	return (local);
}

t_matrix	s_matrix_tuple(t_matrix *m, t_tuple *t)
{
	t_matrix	result;
	ld			tuple_col[4];
	int			i;
	int			k;

	result = (t_matrix){4, 1, NULL, {{(ld){0}}}};
	tuple_col[0] = t->x;
	tuple_col[1] = t->y;
	tuple_col[2] = t->z;
	tuple_col[3] = t->w;
	i = 0;
	while (i < 4)
	{
		k = 0;
		while (k < 4)
		{
			result.stack_matrix[i][0] += m->stack_matrix[i][k] * tuple_col[k];
			k++;
		}
		i++;
	}
	return (result);
}
