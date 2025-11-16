/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_transformation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 22:52:08 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/11 22:52:14 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_tuple	s_cross_product(t_tuple *t1, t_tuple *t2)
{
	t_tuple	cross;

	cross = (t_tuple){0, 0, 0, 0};
	if (!t1 || !t2 || t1->w || t2->w)
		return (cross);
	cross.x = (t1->y * t2->z) - (t1->z * t2->y);
	cross.y = (t1->z * t2->x) - (t1->x * t2->z);
	cross.z = (t1->x * t2->y) - (t1->y * t2->x);
	cross.w = 0;
	return (cross);
}

static t_matrix	build_array(t_v_tran var)
{
	ld	arr[16];

	arr[0] = var.left.x;
	arr[1] = var.left.y;
	arr[2] = var.left.z;
	arr[3] = 0;
	arr[4] = var.true_up.x;
	arr[5] = var.true_up.y;
	arr[6] = var.true_up.z;
	arr[7] = 0;
	arr[8] = -var.forward.x;
	arr[9] = -var.forward.y;
	arr[10] = -var.forward.z;
	arr[11] = 0;
	arr[12] = 0;
	arr[13] = 0;
	arr[14] = 0;
	arr[15] = 1;
	return (s_matrix_constractor(4, 4, arr));
}

t_matrix	view_transformation(t_tuple *from, t_tuple *to, t_tuple *up)
{
	t_matrix	view;
	t_v_tran	var;

	view = (t_matrix){0, 0, NULL, {{(ld){0}}}};
	var.forward = (t_tuple){0, 0, 0, 0};
	s_sub_t(&var.forward, *to, *from);
	s_vec_norm(&var.forward);
	var.upn = *up;
	var.upn.w = 0;
	s_vec_norm(&var.upn);
	var.left = s_cross_product(&var.forward, &var.upn);
	var.true_up = s_cross_product(&var.left, &var.forward);
	var.tmp = build_array(var);
	view = s_matrix_multi(&var.tmp, translation(-from->x, -from->y, -from->z));
	return (view);
}
