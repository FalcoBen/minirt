/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_on_tuple.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:18:08 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:18:11 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_tuple	*tuple_constractor(float x, float y,
		float z, float w)
{
	t_tuple	*t;

	t = alloc(sizeof(t_tuple), false);
	t->w = w;
	t->x = x;
	t->y = y;
	t->z = z;
	return (t);
}

float	magn(t_tuple *t)
{
	if (!t || t->w)
		return (-1);
	return (sqrtf(powf(fabsf(t->x), 2)
			+ powf(fabsf(t->y), 2) + powf(fabsf(t->z), 2)));
}

float	dot_product(t_tuple *t1, t_tuple *t2)
{
	float	dot_res;

	if (!t1 || !t2)
		return (-1);
	dot_res = (t1->x * t2->x) + (t1->y * t2->y) + (t1->z * t2->z);
	return (dot_res);
}

t_tuple	*cross_product(t_tuple *t1, t_tuple *t2)
{
	t_tuple	*cross;

	if (!t1 || !t2 || t1->w || t2->w)
		return (NULL);
	cross = alloc(sizeof(t_tuple), false);
	cross->x = (t1->y * t2->z) - (t1->z * t2->y);
	cross->y = (t1->z * t2->x) - (t1->x * t2->z);
	cross->z = (t1->x * t2->y) - (t1->y * t2->x);
	cross->w = 0;
	return (cross);
}
