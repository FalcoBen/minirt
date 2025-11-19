/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_inverte.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:39:01 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:39:10 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static t_matrix	*create_cofactor_matrix(t_matrix *m)
{
	t_matrix	*c;
	int			i;
	int			j;

	c = matrix_constractor(m->row, m->col, NULL, (size_t) NULL);
	i = 0;
	while (i < m->row)
	{
		j = 0;
		while (j < m->col)
		{
			c->matrix[i][j] = matrix_cofactor(m, i, j);
			j++;
		}
		i++;
	}
	return (c);
}

static t_matrix	*create_inverse_matrix(t_matrix *c, float det, int size)
{
	t_matrix	*invert;
	int			i;
	int			j;

	invert = matrix_constractor(size, size, NULL, (size_t) NULL);
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			invert->matrix[i][j] = c->matrix[i][j] / det;
			j++;
		}
		i++;
	}
	return (invert);
}

static bool	is_matrix_invertible(t_matrix *m, float *det)
{
	if (!m)
		return (false);
	*det = matrix_determinant(m);
	if (fabs(*det) < 0.0001)
	{
		printf("invertible matrix\n");
		return (false);
	}
	return (true);
}

t_matrix	*matrix_inverte(t_matrix *m)
{
	t_matrix	*invert;
	t_matrix	*c;
	float		det;

	if (!is_matrix_invertible(m, &det))
		return (NULL);
	c = create_cofactor_matrix(m);
	c = matrix_transpos(c);
	invert = create_inverse_matrix(c, det, m->row);
	copy_matrix_contant(invert);
	return (invert);
}
