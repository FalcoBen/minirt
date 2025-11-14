/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_computations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 23:30:36 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/14 05:10:45 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

t_material	*get_material(t_comp *comp)
{
	t_sphere	*sphere;
	t_cylinder	*cylinder;
	t_cone		*cone;
	t_plane		*plane;

	if (!comp)
		return (NULL);
	if (comp->obj_type == T_SPHERE)
	{
		sphere = (t_sphere *)comp->obj;
		return (sphere->material);
	}
	else if (comp->obj_type == T_CYLINDRE)
	{
		cylinder = (t_cylinder *)comp->obj;
		return (cylinder->material);
	}
	else if (comp->obj_type == T_CONE)
	{
		cone = (t_cone *)comp->obj;
		return (cone->material);
	}
	else if (comp->obj_type == T_PLAN)
	{
		plane = (t_plane *)comp->obj;
		return (plane->material);
	}
	return (NULL);
}

void	get_uv_coors(t_comp *comp, ld *u, ld *v)
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


t_color shade_helper(t_material *material, t_comp *comp)
{
    t_color base_color;
    ld u;
	ld v;
	
	base_color = *(material->color);
	get_uv_coors(comp, &u, &v);
    if (material->has_color_texture 
        && material->color_texture 
        && material->color_texture->type == 1)
    {
        t_vec3 tex_color = sample_color(material->color_texture, u, v);
        base_color.r = tex_color.x;
        base_color.g = tex_color.y;
        base_color.b = tex_color.z;
    }
    else if (material->has_bump_map 
        && material->bump_map 
        && material->bump_map->type == 2 
        && material->bump_map->image)
    {
        t_vec3 tex_color = sample_color(material->bump_map, u, v);
        base_color.r = tex_color.x;
        base_color.g = tex_color.y;
        base_color.b = tex_color.z;
    }
    
    return base_color;
}

t_color	shade_hit(t_world *world, t_comp *comp)
{
	t_color		final_color;
	t_material	*material;
	t_material	temp_material;
	t_color		base_color;
	t_color		light_contribution;
	bool 		shadowed;
	int			i;

	final_color = (t_color){0, 0, 0};
	material = get_material(comp);
	if (!material || !world || !comp)
		return (final_color);
	temp_material = *material;
	base_color = shade_helper(material, comp);
	temp_material.color = &base_color;
	i = 0;
	while (i < world->light_num)
	{
		shadowed = is_shadowed(world, comp->over_point, world->lights[i]);
		light_contribution = s_lighting(temp_material, *(world->lights[i]),
				comp->over_point, comp->eyev,
				comp->normalv, shadowed);
		final_color.r += light_contribution.r;
		final_color.g += light_contribution.g;
		final_color.b += light_contribution.b;
		i++;
	}
	if (final_color.r > 1.0)
		final_color.r = 1.0;
	else if (final_color.r < 0.0)
		final_color.r = 0.0;
	else
		final_color.r = final_color.r;
	if (final_color.g > 1.0)
		final_color.g = 1.0;
	else if (final_color.g < 0.0)
		final_color.g = 0.0;
	else
		final_color.g = final_color.g;
	if (final_color.b > 1.0)
		final_color.b = 1.0;
	else if (final_color.b < 0.0)
		final_color.b = 0.0;
	else
		final_color.b = final_color.b;
	return (final_color);
}


t_tuple	s_position(t_stack_ray *ray, ld t)
{
	t_tuple	pos;
	t_tuple	scalar_res;

	s_scalar_mult(&scalar_res, ray->vect, t);
	s_add_t(&pos, ray->point, scalar_res);
	return (pos);
}

void	compute_normal_at(t_tuple *normal,
	t_inters *intersection, t_tuple *point)
{
	if (intersection->sphere != NULL)
		s_normal_at(normal, intersection->sphere, point);
	else if (intersection->cylinder != NULL)
		s_cylinder_normal(normal, intersection->cylinder, point);
	else if (intersection->cone != NULL)
		s_cone_normal(normal, intersection->cone, point);
	else if (intersection->plane != NULL)
		s_plane_normal(normal, intersection->plane, point);
}

t_comp	prepare_computations(t_inters *intersection, t_stack_ray *ray)
{
	t_comp	new;
	t_tuple	normalv;
	t_tuple	scaled_normal;

	new = (t_comp){NULL, T_SPHERE, {0, 0, 0, 0}, {0, 0, 0, 0},
	{0, 0, 0, 0}, {0, 0, 0, 0}, NULL, 0, false};
	if (!intersection || !ray)
		return (new);
	if (intersection->sphere != NULL)
	{
		new.obj = intersection->sphere;
		new.obj_type = T_SPHERE;
	}
	else if (intersection->cylinder != NULL)
	{
		new.obj = intersection->cylinder;
		new.obj_type = T_CYLINDRE;
	}
	else if (intersection->cone != NULL)
	{
		new.obj = intersection->cone;
		new.obj_type = T_CONE;
	}
	else if (intersection->plane != NULL)
	{
		new.obj = intersection->plane;
		new.obj_type = T_PLAN;
	}
	new.intersection = intersection;
	new.t = intersection->inters_value;
	new.point = s_position(ray, intersection->inters_value);
	new.eyev = s_neg_t(ray->vect);
	compute_normal_at(&normalv, intersection, &new.point);
	s_vec_norm(&normalv);
	if (dot_product(&normalv, &new.eyev) < 0)
	{
		new.inside = true;
		new.normalv = s_neg_t(normalv);
	}
	else
	{
		new.inside = false;
		new.normalv = normalv;
	}
	s_scalar_mult(&scaled_normal, new.normalv, SHADOW_EPSILON);
	s_add_t(&new.over_point, new.point, scaled_normal);
	return (new);
}
