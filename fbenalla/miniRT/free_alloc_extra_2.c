/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_alloc_extra_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:07:46 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:37:24 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_params_co(t_cone_fb *co)
{
	free(co->color_cone);
	free(co->coor_cone);
	free(co->vector_cone);
	free(co);
}

void	free_params_cy(t_cylinder_fb *cy)
{
	free(cy->color_cylinder);
	free(cy->coor_cylinder);
	free(cy->vector_cylinder);
	free(cy);
}

void	free_params_pl(t_plane_fb *pl)
{
	free(pl->color_plane);
	free(pl->coor_plane);
	free(pl->vector_plane);
	free(pl);
}

void	free_params_sp(t_sphere_fb *sp)
{
	free(sp->color_sphere);
	free(sp->coor_sphere);
	free(sp);
}
