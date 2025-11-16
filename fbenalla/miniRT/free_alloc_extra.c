/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_alloc_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:05:48 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:35:31 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_sphere(t_scene *scene)
{
	t_sphere_fb			*sp;
	t_sphere_fb			*next_sp;

	sp = scene->sphere;
	while (sp)
	{
		next_sp = sp->next;
		if (sp->flag_bump && sp->bump_texture)
		{
			if (sp->bump_texture->image)
			{
				if (sp->bump_texture->type == 2)
					mlx_delete_texture(sp->bump_texture->image);
				sp->bump_texture->image = NULL;
			}
		}
		sp = next_sp;
	}
}

void	free_plane(t_scene *scene)
{
	t_plane_fb			*pl;
	t_plane_fb			*next_pl;

	pl = scene->plane;
	while (pl)
	{
		next_pl = pl->next;
		if (pl->flag_bump && pl->bump_texture)
		{
			if (pl->bump_texture->image && pl->bump_texture->type == 2)
			{
				mlx_delete_texture(pl->bump_texture->image);
				pl->bump_texture->image = NULL;
			}
		}
		pl = next_pl;
	}
}

void	free_cylinder(t_scene *scene)
{
	t_cylinder_fb		*cy;
	t_cylinder_fb		*next_cy;

	cy = scene->cylinder;
	while (cy)
	{
		next_cy = cy->next;
		if (cy->flag_bump && cy->bump_texture)
		{
			if (cy->bump_texture->image && cy->bump_texture->type == 2)
			{
				mlx_delete_texture(cy->bump_texture->image);
				cy->bump_texture->image = NULL;
			}
		}
		cy = next_cy;
	}
}

void	free_cone(t_scene *scene)
{
	t_cone_fb		*co;
	t_cone_fb		*next_co;

	co = scene->cone;
	while (co)
	{
		next_co = co->next;
		if (co->flag_bump && co->bump_texture)
		{
			if (co->bump_texture->image && co->bump_texture->type == 2)
			{
				mlx_delete_texture(co->bump_texture->image);
				co->bump_texture->image = NULL;
			}
		}
		co = next_co;
	}
}
