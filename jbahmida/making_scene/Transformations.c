/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Transformations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:59:01 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:59:09 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_matrix	*translation(float x, float y, float z)
{
	t_matrix	*m;

	m = identity_matrix(4, 4);
	m->matrix[0][3] = x;
	m->matrix[1][3] = y;
	m->matrix[2][3] = z;
	copy_matrix_contant(m);
	return (m);
}

t_matrix	*scaling(float x, float y, float z)
{
	t_matrix	*m;

	m = identity_matrix(4, 4);
	m->matrix[0][0] = x;
	m->matrix[1][1] = y;
	m->matrix[2][2] = z;
	copy_matrix_contant(m);
	return (m);
}

t_matrix	*rotation_x(float angle)
{
	t_matrix	*m_x;

	m_x = identity_matrix(4, 4);
	m_x->matrix[1][1] = cosf(angle);
	m_x->matrix[1][2] = -sinf(angle);
	m_x->matrix[2][1] = sinf(angle);
	m_x->matrix[2][2] = cosf(angle);
	copy_matrix_contant(m_x);
	return (m_x);
}

t_matrix	*rotation_y(float angle)
{
	t_matrix	*m_y;

	m_y = identity_matrix(4, 4);
	m_y->matrix[0][0] = cosf(angle);
	m_y->matrix[0][2] = sinf(angle);
	m_y->matrix[2][0] = -sinf(angle);
	m_y->matrix[2][2] = cosf(angle);
	copy_matrix_contant(m_y);
	return (m_y);
}

t_matrix	*rotation_z(float angle)
{
	t_matrix	*m_z;

	m_z = identity_matrix(4, 4);
	m_z->matrix[0][0] = cosf(angle);
	m_z->matrix[0][1] = -sinf(angle);
	m_z->matrix[1][0] = sinf(angle);
	m_z->matrix[1][1] = cosf(angle);
	copy_matrix_contant(m_z);
	return (m_z);
}
