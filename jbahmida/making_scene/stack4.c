/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:00:52 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:00:53 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_tuple	s_create_tuple(ld *coord, int type)
{
	t_tuple	local;

	local = (t_tuple){0, 0, 0, 0};
	if (!coord)
		return (local);
	local.x = coord[0];
	local.y = coord[1];
	local.z = coord[2];
	if (type == 1)
		local.w = 1;
	else
		local.w = 0;
	return (local);
}

t_tuple	s_matrix_to_tuple(t_matrix *m)
{
	t_tuple	local;

	local = (t_tuple){0, 0, 0, 0};
	local = s_create_tuple(NULL, -1);
	if (m->row != 4 || m->col != 1)
		return (local);
	local.x = m->stack_matrix[0][0];
	local.y = m->stack_matrix[1][0];
	local.z = m->stack_matrix[2][0];
	local.w = m->stack_matrix[3][0];
	return (local);
}

t_tuple	s_matrix_to_tuple1(t_matrix *m)
{
	t_tuple	local;

	local = (t_tuple){0, 0, 0, 0};
	local = s_create_tuple(NULL, -1);
	if (m->row != 4 || m->col != 1)
		return (local);
	local.x = m->stack_matrix[0][0];
	local.y = m->stack_matrix[1][0];
	local.z = m->stack_matrix[2][0];
	local.w = m->stack_matrix[3][0];
	return (local);
}
