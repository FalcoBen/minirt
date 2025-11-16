/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:04:45 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:04:46 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_tuple	s_neg_t(t_tuple t)
{
	t_tuple	local;

	local = (t_tuple){0, 0, 0, 0};
	local.x = -(t.x);
	local.y = -(t.y);
	local.z = -(t.z);
	local.w = 0;
	return (local);
}

void	s_neg_t1(t_tuple *t)
{
	t->x = -(t->x);
	t->y = -(t->y);
	t->z = -(t->z);
	t->w = 0;
}

t_tuple	s_reflect(t_tuple in, t_tuple normal)
{
	t_tuple	local;
	t_tuple	local1;
	ld		t;

	local = (t_tuple){0, 0, 0, 0};
	local1 = (t_tuple){0, 0, 0, 0};
	t = (2 * dot_product(&in, &normal));
	s_scalar_mult(&local1, normal, t);
	s_sub_t(&local, in, local1);
	return (local);
}

void	s_add_t(t_tuple *res, t_tuple t1, t_tuple t2)
{
	res->x = t1.x + t2.x;
	res->y = t1.y + t2.y;
	res->z = t1.z + t2.z;
	res->w = t1.w + t2.w;
	return ;
}

void	s_sub_t(t_tuple *res, t_tuple t1, t_tuple t2)
{
	res->x = t1.x - t2.x;
	res->y = t1.y - t2.y;
	res->z = t1.z - t2.z;
	res->w = 0;
	return ;
}
