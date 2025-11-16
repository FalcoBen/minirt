/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:56:52 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/16 02:56:55 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	plane_uv_checker(t_tuple *point, t_plane *plane, float *u, float *v)
{
	t_matrix	m;
	t_tuple		loc;
	float		scale;

	m = s_matrix_tuple(plane->inv, point);
	loc = s_matrix_to_tuple(&m);
	scale = 10.0;
	*u = loc.x / scale;
	*v = loc.z / scale;
	*u = fmod(*u, 1.0);
	*v = fmod(*v, 1.0);
	if (*u < 0)
		*u += 1.0;
	if (*v < 0)
		*v += 1.0;
}

void	plane_uv_image(t_tuple *point, t_plane *plane, float *u, float *v)
{
	t_matrix	m;
	t_tuple		loc;
	float		scale;

	m = s_matrix_tuple(plane->inv, point);
	loc = s_matrix_to_tuple(&m);
	scale = 12.0;
	*u = loc.x / scale;
	*v = loc.z / scale;
	*u = *u * 0.5 + 0.5;
	*v = *v * 0.5 + 0.5;
	*u = fmax(0.0, fmin(1.0, *u));
	*v = fmax(0.0, fmin(1.0, *v));
}

void	cy_and_cone_count(t_cylinder_fb *cy, t_cone_fb *co, int *counter)
{
	while (cy)
	{
		(*counter)++;
		cy = cy->next;
	}
	while (co)
	{
		(*counter)++;
		co = co->next;
	}
}

int	number_of_all_objects(t_scene *scene)
{
	t_sphere_fb		*sp;
	t_plane_fb		*pl;
	t_cylinder_fb	*cy;
	t_cone_fb		*co;
	int				counter;

	counter = 0;
	sp = scene->sphere;
	pl = scene->plane;
	cy = scene->cylinder;
	co = scene->cone;
	while (sp)
	{
		counter++;
		sp = sp->next;
	}
	while (pl)
	{
		counter++;
		pl = pl->next;
	}
	cy_and_cone_count(cy, co, &counter);
	return (counter);
}

void	get_uv_coors(t_comp *comp, float *u, float *v)
{
	if (comp->obj_type == T_SPHERE)
		sphere_uv(&comp->point, (t_sphere *)comp->obj, u, v);
	else if (comp->obj_type == T_PLAN)
		plane_uv(&comp->point, (t_plane *)comp->obj, u, v);
	else if (comp->obj_type == T_CYLINDRE)
		cylinder_uv(&comp->point, (t_cylinder *)comp->obj, u, v);
	else if (comp->obj_type == T_CONE)
		cone_uv(&comp->point, (t_cone *)comp->obj, u, v);
	return ;
}
