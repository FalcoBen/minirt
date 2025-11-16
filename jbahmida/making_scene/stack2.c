/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:53:32 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 22:53:37 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	s_scalar_mult(t_tuple *res, t_tuple t, ld scalar)
{
	res->x = t.x * scalar;
	res->y = t.y * scalar;
	res->z = t.z * scalar;
	res->w = t.w;
	return ;
}

t_matrix	s_matrix_transpos(t_matrix *m)
{
	t_matrix	*tmp;
	t_matrix	tran;
	int			i;
	int			j;

	tmp = m;
	i = 0;
	while (i < m->row)
	{
		j = 0;
		while (j < m->col)
		{
			tran.stack_matrix[j][i] = tmp->matrix[i][j];
			j++;
		}
		i++;
	}
	return (tran);
}

void	s_vec_norm(t_tuple *t)
{
	ld	mgn;

	if (!magn(t))
		return ;
	mgn = magn(t);
	t->x = t->x / mgn;
	t->y = t->y / mgn;
	t->z = t->z / mgn;
	t->w = 0;
}
