/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:03:33 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/13 22:03:47 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	cone(t_cone **cone, t_matrix *trans, bool flag)
{
	static int	id;

	id = 0;
	if (!cone)
		return ;
	if (flag)
	{
		(*cone)->transform = trans;
		(*cone)->inv = matrix_inverte((*cone)->transform);
		return ;
	}
	(*cone) = alloc(sizeof(t_cone), false);
	(*cone)->transform = identity_matrix(4, 4);
	(*cone)->inv = identity_matrix(4, 4);
	(*cone)->id = id;
	(*cone)->material = material();
	id++;
}

t_matrix	*rotation_from_to(t_tuple *from, t_tuple *to)
{
	t_matrix	*result;
	t_tuple		v;
	ld			factor;
	ld			s;
	ld			c;
	ld			vx[9];
	int			i;
	int			j;
	int			k;

	v = s_cross_product(from, to);
	s = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	c = dot_product(from, to);
	if (s < 0.0001)
	{
		if (c > 0)
			return (identity_matrix(4, 4));
		else
			return (rotation_x((M_PI)));
	}	
	vx[0] = 0.0;
	vx[1] = -v.z;
	vx[2] = v.y;
	vx[3] = v.z;
	vx[4] = 0.0;
	vx[5] = -v.x;
	vx[6] = -v.y;
	vx[7] = v.x;
	vx[8] = 0.0;

	result = identity_matrix(4, 4);
	factor = (1 - c) / (s * s);
	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			k = 0;
			result->matrix[i][j] += vx[i*3 + j];
			while (k < 3)
			{
				result->matrix[i][j] += vx[(i * 3) + k]
					* vx[(k * 3) + j] * factor;
				k++;
			}
			j++;
		}
		i++;
	}

	return (result);
}

t_matrix	*create_cone_transform(t_tuple *position, t_tuple *axis, 
		ld radius, ld height)
{
	t_tuple normalized_axis;
	t_tuple default_y;
	t_matrix *rotation;
	t_matrix *scale;
	t_matrix *trans;
	t_matrix *temp;
	t_matrix *result;

	normalized_axis = *axis;
	s_vec_norm(&normalized_axis);
	default_y = (t_tuple){0, 1, 0, 0};
	rotation = rotation_from_to(&default_y, &normalized_axis);
	scale = scaling(radius, height / 2.0, radius);
	trans = translation(position->x, position->y, position->z);
	temp = matrix_multi(trans, rotation);
	result = matrix_multi(temp, scale);
	copy_matrix_contant(result);
	return (result);
}

bool	check_cap_cone(t_stack_ray *ray, ld t, ld y)
{
	ld	x;
	ld	z;

	x = ray->point.x + t * ray->vect.x;
	z = ray->point.z + t * ray->vect.z;
	return ((powf(x, 2) + powf(z, 2)) <= fabsf(y));
}

void	intersect_caps_cone(t_cone *cone,
	t_stack_ray *ray, t_stack_intersections *xs)
{
	ld	t;

	if (!cone->closed || ldbl_cmp(ray->vect.y, 0.0))
		return ;
	t = (cone->minimum - ray->point.y) / ray->vect.y;
	if (check_cap_cone(ray, t, cone->minimum))
	{
		xs->inters_list[xs->count].inters_value = t;
		xs->inters_list[xs->count].sphere = NULL;
		xs->inters_list[xs->count].plane = NULL;
		xs->inters_list[xs->count].cylinder = NULL;
		xs->inters_list[xs->count].cone = cone;
		xs->inters_list[xs->count].existence = true;
		xs->inters_list[xs->count].type = T_CONE;
		xs->count++;
	}
	t = (cone->maximum - ray->point.y) / ray->vect.y;
	if (check_cap_cone(ray, t, cone->maximum))
	{
		xs->inters_list[xs->count].inters_value = t;
		xs->inters_list[xs->count].sphere = NULL;
		xs->inters_list[xs->count].plane = NULL;
		xs->inters_list[xs->count].cylinder = NULL;
		xs->inters_list[xs->count].cone = cone;
		xs->inters_list[xs->count].existence = true;
		xs->inters_list[xs->count].type = T_CONE;
		xs->count++;
	}
}

void	cone_intersct(t_stack_intersections *xs, t_cone *cone, t_stack_ray *ray)
{
	t_stack_ray	local_ray;
	ld			a;
	ld			b;
	ld			c;
	ld			delta;
	ld			t;
	ld			t0;
	ld			t1;
	ld			tmp;
	ld			y;
	ld			y0;
	ld			y1;

	local_ray = s_transform(ray, cone->inv);
	a = powf(local_ray.vect.x, 2)
		- powf(local_ray.vect.y, 2) + powf(local_ray.vect.z, 2);
	b = 2 * local_ray.point.x * local_ray.vect.x
		- 2 * local_ray.point.y * local_ray.vect.y
		+ 2 * local_ray.point.z * local_ray.vect.z;
	c = powf(local_ray.point.x, 2)
		- powf(local_ray.point.y, 2) + powf(local_ray.point.z, 2);
	xs->count = 0;
	xs->existence = false;
	if (ldbl_cmp(a, 0.0))
	{
		if (!ldbl_cmp(b, 0.0))
		{
			t = -c / (2 * b);
			y = local_ray.point.y + t * local_ray.vect.y;
			if (y > cone->minimum && y < cone->maximum)
			{
				xs->inters_list[xs->count].inters_value = t;
				xs->inters_list[xs->count].sphere = NULL;
				xs->inters_list[xs->count].cylinder = NULL;
				xs->inters_list[xs->count].plane = NULL;
				xs->inters_list[xs->count].cone = cone;
				xs->inters_list[xs->count].existence = true;
				xs->inters_list[xs->count].type = T_CONE;
				xs->count++;
			}
		}
	}
	else
	{
		delta = powf(b, 2) - 4 * a * c;
		if (delta >= 0)
		{
			t0 = ((-b) - sqrtl(delta)) / (2 * a);
			t1 = ((-b) + sqrtl(delta)) / (2 * a);
			if (t0 > t1)
			{
				tmp = t0;
				t0 = t1;
				t1 = tmp;
			}
			y0 = local_ray.point.y + t0 * local_ray.vect.y;
			if (y0 > cone->minimum && y0 < cone->maximum)
			{
				xs->inters_list[xs->count].inters_value = t0;
				xs->inters_list[xs->count].sphere = NULL;
				xs->inters_list[xs->count].cylinder = NULL;
				xs->inters_list[xs->count].plane = NULL;
				xs->inters_list[xs->count].cone = cone;
				xs->inters_list[xs->count].existence = true;
				xs->inters_list[xs->count].type = T_CONE;
				xs->count++;
			}
			y1 = local_ray.point.y + t1 * local_ray.vect.y;
			if (y1 > cone->minimum && y1 < cone->maximum)
			{
				xs->inters_list[xs->count].inters_value = t1;
				xs->inters_list[xs->count].sphere = NULL;
				xs->inters_list[xs->count].cylinder = NULL;
				xs->inters_list[xs->count].plane = NULL;
				xs->inters_list[xs->count].cone = cone;
				xs->inters_list[xs->count].existence = true;
				xs->inters_list[xs->count].type = T_CONE;
				xs->count++;
			}
		}
	}

	intersect_caps_cone(cone, &local_ray, xs);

	xs->existence = xs->count > 0;

	if (!xs->existence)
		xs->world_inters_list = NULL;
}

void	s_cone_normal(t_tuple *norm, t_cone *cone, t_tuple *point)
{
	t_matrix	transpose;
	t_matrix	world_normal;
	t_matrix	point_matrix;
	t_tuple		local_point;
	t_tuple		local_normal;
	ld			dist;
	ld			y;

	point_matrix = s_matrix_tuple(cone->inv, point);
	local_point = s_matrix_to_tuple(&point_matrix);
	dist = powf(local_point.x, 2) + powf(local_point.z, 2);
	if (cone->closed && dist < fabsf(cone->maximum)
		&& local_point.y >= cone->maximum - FLT_EPSILON)
	{
		local_normal = (t_tuple){0, 1, 0, 0};
	}
	else if (cone->closed && dist < fabsf(cone->minimum)
		&& local_point.y <= cone->minimum + FLT_EPSILON)
	{
		local_normal = (t_tuple){0, -1, 0, 0};
	}
	else
	{
		y = sqrtl(powf(local_point.x, 2) + powf(local_point.z, 2));
		if (local_point.y > 0)
			y = -y;
		local_normal.x = local_point.x;
		local_normal.y = y;
		local_normal.z = local_point.z;
		local_normal.w = 0;
	}
	transpose = s_matrix_transpos(cone->inv);
	world_normal = s_matrix_tuple(&transpose, &local_normal);
	*norm = s_matrix_to_tuple(&world_normal);
	norm->w = 0;
	s_vec_norm(norm);
}


