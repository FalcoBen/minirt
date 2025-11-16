/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp_foalt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:01:19 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 02:01:25 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

bool	ldbl_cmp(ld a, ld b)
{
	ld	diff;
	ld	largest;
	ld	abs_epsilon;
	ld	rel_epsilon;

	diff = fabsl(a - b);
	largest = fmaxl(fabsl(a), fabsl(b));
	abs_epsilon = 1e-18L;
	rel_epsilon = 1e-15L;
	return (diff <= fmaxl(abs_epsilon, largest * rel_epsilon));
}
