/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_cylinder_transform.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 02:10:23 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 02:10:27 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static void	compute_rotation_element(t_matrix *rot, t_roaa *var, int i, int j)
{
	float	components[3];

	components[0] = var->norm_axis.x;
	components[1] = var->norm_axis.y;
	components[2] = var->norm_axis.z;
	if (i == j)
		var->diag_part = var->cos_theta;
	else
		var->diag_part = 0.0;
	if (i < 3 && j < 3)
		var->outer = (1.0 - var->cos_theta)
			* components[i] * components[j];
	else
		var->outer = 0.0;
	rot->matrix[i][j] = var->diag_part
		+ var->outer
		+ var->skew->matrix[i][j] * var->sin_theta;
}

static void	build_rotation(t_matrix *rot, t_roaa *var)
{
	int		i;
	int		j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			compute_rotation_element(rot, var, i, j);
			j++;
		}
		i++;
	}
}

void	skew_symmetric(t_tuple *axis, t_matrix *result)
{
	result->matrix[0][0] = 0;
	result->matrix[0][1] = -axis->z;
	result->matrix[0][2] = axis->y;
	result->matrix[0][3] = 0;
	result->matrix[1][0] = axis->z;
	result->matrix[1][1] = 0;
	result->matrix[1][2] = -axis->x;
	result->matrix[1][3] = 0;
	result->matrix[2][0] = -axis->y;
	result->matrix[2][1] = axis->x;
	result->matrix[2][2] = 0;
	result->matrix[2][3] = 0;
	result->matrix[3][0] = 0;
	result->matrix[3][1] = 0;
	result->matrix[3][2] = 0;
	result->matrix[3][3] = 1;
}

t_matrix	*rotation_axis_angle(t_tuple *axis, float angle)
{
	t_matrix	*rot;
	t_roaa		var;

	rot = identity_matrix(4, 4);
	var.norm_axis = *axis;
	s_vec_norm(&var.norm_axis);
	var.skew = matrix_constractor(4, 4, (float [16]){0}, 16);
	skew_symmetric(&var.norm_axis, var.skew);
	var.cos_theta = cos(angle);
	var.sin_theta = sin(angle);
	build_rotation(rot, &var);
	return (rot);
}

t_matrix	*create_cylinder_transform(t_tuple *position, t_tuple *axis,
		float radius, float height)
{
	t_nor_cy_tran	var;

	var.normalized_axis = *axis;
	s_vec_norm(&var.normalized_axis);
	var.default_y = (t_tuple){0, 1, 0, 0};
	var.dot = dot_product(&var.default_y, &var.normalized_axis);
	var.dot = fmax(fmin(var.dot, 1.0), -1.0);
	var.rotation = NULL;
	if (fabs(var.dot - 1.0) < 1e-3)
		var.rotation = identity_matrix(4, 4);
	else if (fabs(var.dot + 1.0) < 1e-3)
		var.rotation = rotation_x(M_PI);
	else
	{
		var.rot_axis = *cross_product(&var.default_y, &var.normalized_axis);
		s_vec_norm(&var.rot_axis);
		var.angle = acos(var.dot);
		var.rotation = rotation_axis_angle(&var.rot_axis, var.angle);
	}
	var.scale = scaling(radius, height / 2.0, radius);
	var.trans = translation(position->x, position->y, position->z);
	var.temp = matrix_multi(var.trans, var.rotation);
	var.result = matrix_multi(var.temp, var.scale);
	copy_matrix_contant(var.result);
	return (var.result);
}

