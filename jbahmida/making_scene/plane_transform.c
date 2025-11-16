/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_transform.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 03:53:45 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/15 03:53:49 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

static t_matrix	*handle_special_cases(t_tuple *normalized_normal)
{
	t_matrix	*rotation;

	rotation = NULL;
	if (fabsf(normalized_normal->x) > 0.999)
	{
		if (normalized_normal->x > 0)
			rotation = rotation_z(-M_PI / 2);
		else
			rotation = rotation_z(M_PI / 2);
	}
	else if (fabsf(normalized_normal->z) > 0.999)
	{
		if (normalized_normal->z > 0)
			rotation = rotation_x(M_PI / 2);
		else
			rotation = rotation_x(-M_PI / 2);
	}
	else
		rotation = identity_matrix(4, 4);
	return (rotation);
}

static t_matrix	*create_rotation_matrix(t_tuple *normalized_normal)
{
	t_tuple		default_y;
	float		dot;
	t_matrix	*rotation;
	t_tuple		axis;
	float		angle;

	default_y = (t_tuple){0, 1, 0, 0};
	dot = dot_product(&default_y, normalized_normal);
	if (fabsf(dot - 1.0) < 0.001)
		rotation = identity_matrix(4, 4);
	else if (fabsf(dot + 1.0) < 0.001)
		rotation = rotation_x(M_PI);
	else
		rotation = handle_special_cases(normalized_normal);
	return (rotation);
}

static t_matrix	*create_translation_matrix(t_tuple *origin)
{
	return (translation(origin->x, origin->y, origin->z));
}

t_matrix	*create_plane_transform(t_tuple *origin, t_tuple *normal)
{
	t_tuple		normalized_normal;
	t_matrix	*rotation;
	t_matrix	*trans;
	t_matrix	*result;

	normalized_normal = *normal;
	s_vec_norm(&normalized_normal);
	rotation = create_rotation_matrix(&normalized_normal);
	trans = create_translation_matrix(origin);
	result = matrix_multi(trans, rotation);
	copy_matrix_contant(result);
	return (result);
}
