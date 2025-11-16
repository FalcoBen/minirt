/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:04:35 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 01:04:39 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

ld	tone_map(ld x)
{
	return (x / (1.0 + x));
}

ld	gamma_correct(ld x, ld gamma)
{
	return (pow(x, 1.0 / gamma));
}

t_color	s_hadamard(t_color *c1, t_color *c2)
{
	t_color	local;

	local = (t_color){0, 0, 0};
	local.r = c1->r * c2->r;
	local.g = c1->g * c2->g;
	local.b = c1->b * c2->b;
	return (local);
}

t_color	s_color_scalar(t_color *c, ld s)
{
	t_color	local;

	local = (t_color){0, 0, 0};
	local.r = c->r * s;
	local.g = c->g * s;
	local.b = c->b * s;
	return (local);
}

t_color	s_color_add(t_color *c1, t_color *c2)
{
	t_color	local;

	local = (t_color){0, 0, 0};
	local.r = c1->r + c2->r;
	local.g = c1->g + c2->g;
	local.b = c1->b + c2->b;
	return (local);
}
