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

// t_matrix	*create_cone_transform(t_tuple *position, t_tuple *axis,
// 	float radius, float height)
// {
// 	t_nor_cone_tran	var;

// 	var.normalized_axis = *axis;
// 	s_vec_norm(&var.normalized_axis);
// 	var.default_y = (t_tuple){0, 1, 0, 0};
// 	var.dot = dot_product(&var.default_y, &var.normalized_axis);
// 	var.dot = fmax(fmin(var.dot, 1.0), -1.0);
// 	var.rotation = NULL;
// 	if (fabs(var.dot - 1.0) < 1e-3)
// 		var.rotation = identity_matrix(4, 4);
// 	else if (fabs(var.dot + 1.0) < 1e-3)
// 		var.rotation = rotation_x(M_PI);
// 	else
// 	{
// 		var.rot_axis = *cross_product(&var.default_y, &var.normalized_axis);
// 		s_vec_norm(&var.rot_axis);
// 		var.angle = acos(var.dot);
// 		var.rotation = rotation_axis_angle(&var.rot_axis, var.angle);
// 	}
// 	var.scale = scaling(radius, height / 2.0, radius);
// 	var.trans = translation(position->x, position->y, position->z);
// 	var.temp = matrix_multi(var.trans, var.rotation);
// 	var.result = matrix_multi(var.temp, var.scale);
// 	copy_matrix_contant(var.result);
// 	return (var.result);
// }
// t_matrix	*create_cone_transform(t_tuple *position, t_tuple *axis,
// 	float angle_degrees, float height)
// {
// 	t_nor_cone_tran	var;
// 	float			angle_radians;

// 	printf("\n### CREATE_CONE_TRANSFORM DEBUG ###\n");
// 	printf("Position: (%f, %f, %f)\n", position->x, position->y, position->z);
// 	printf("Axis (raw): (%f, %f, %f)\n", axis->x, axis->y, axis->z);
// 	printf("Angle: %f degrees, Height: %f\n", angle_degrees, height);
	
// 	// Normalize the axis
// 	var.normalized_axis = *axis;
// 	s_vec_norm(&var.normalized_axis);
	
// 	printf("Axis (normalized): (%f, %f, %f)\n", 
// 		var.normalized_axis.x, var.normalized_axis.y, var.normalized_axis.z);
	
// 	var.default_y = (t_tuple){0, 1, 0, 0};
// 	var.dot = dot_product(&var.default_y, &var.normalized_axis);
// 	var.dot = fmax(fmin(var.dot, 1.0), -1.0);
	
// 	var.rotation = NULL;
// 	if (fabs(var.dot - 1.0) < 1e-3)
// 		var.rotation = identity_matrix(4, 4);
// 	else if (fabs(var.dot + 1.0) < 1e-3)
// 		var.rotation = rotation_x(M_PI);
// 	else
// 	{
// 		var.rot_axis = *cross_product(&var.default_y, &var.normalized_axis);
// 		s_vec_norm(&var.rot_axis);
// 		var.angle = acos(var.dot);
// 		var.rotation = rotation_axis_angle(&var.rot_axis, var.angle);
// 	}
	
// 	// CRITICAL FIX: The cone equation is x² + z² = y²
// 	// At y=1, radius=1 (for 45° cone)
// 	// We need to scale uniformly in X and Z
// 	angle_radians = angle_degrees * M_PI / 180.0;
// 	float scale_xz = tanf(angle_radians);  // For 45°, this is 1.0
	
// 	printf("Scale: XZ=%f, Y=%f\n", scale_xz, height / 2.0);
	
// 	var.scale = scaling(scale_xz, height / 2.0, scale_xz);
	
// 	// Translation moves the CENTER of the cone
// 	// But the cone needs to be positioned so its base is at the right place
// 	// Adjust position to account for the fact that the cone is centered at origin in object space
// 	float center_offset = (cone->maximum + cone->minimum) / 2.0;
	
// 	// We need to translate along the axis direction
// 	t_tuple offset_vec = var.normalized_axis;
// 	s_scalar_mult(&offset_vec, offset_vec, center_offset);
	
// 	var.trans = translation(
// 		position->x + offset_vec.x, 
// 		position->y + offset_vec.y, 
// 		position->z + offset_vec.z
// 	);
	
// 	var.temp = matrix_multi(var.trans, var.rotation);
// 	var.result = matrix_multi(var.temp, var.scale);
	
// 	printf("### END TRANSFORM ###\n\n");
	
// 	copy_matrix_contant(var.result);
// 	return (var.result);
// }

bool	check_cap_cone(t_stack_ray *ray, float t, float y)
{
	float	x;
	float	z;
	float	radius;

	x = ray->point.x + t * ray->vect.x;
	z = ray->point.z + t * ray->vect.z;
	radius = fabsf(y);
	return ((powf(x, 2) + powf(z, 2)) <= powf(radius, 2));
}

static void	set_cap_value(t_stack_intersections *xs,
	t_cone *cone, float t)
{
	xs->inters_list[xs->count].inters_value = t;
	xs->inters_list[xs->count].sphere = NULL;
	xs->inters_list[xs->count].plane = NULL;
	xs->inters_list[xs->count].cylinder = NULL;
	xs->inters_list[xs->count].cone = cone;
	xs->inters_list[xs->count].existence = true;
	xs->inters_list[xs->count].type = T_CONE;
	xs->count++;
	return ;
}

// void	intersect_caps_cone(t_cone *cone,
// 	t_stack_ray *ray, t_stack_intersections *xs)
// {
// 	float	t;

// 	if (!cone->closed || ldbl_cmp(ray->vect.y, 0.0))
// 		return ;
// 	t = (cone->minimum - ray->point.y) / ray->vect.y;
// 	if (check_cap_cone(ray, t, cone->minimum))
// 		set_cap_value(xs, cone, t);
// 	t = (cone->maximum - ray->point.y) / ray->vect.y;
// 	if (check_cap_cone(ray, t, cone->maximum))
// 		set_cap_value(xs, cone, t);
// 	return ;
// }

void	intersect_caps_cone(t_cone *cone,
	t_stack_ray *ray, t_stack_intersections *xs)
{
	float	t;
	float	x, z, radius, dist;

	// printf("  >> Checking caps: closed=%d, ray.y=%f\n", cone->closed, ray->vect.y);
	
	if (!cone->closed || ldbl_cmp(ray->vect.y, 0.0))
	{
		// printf("  >> Caps check skipped (closed=%d, ray.y≈0=%d)\n", 
		// 	cone->closed, ldbl_cmp(ray->vect.y, 0.0));
		return ;
	}
	
	// Bottom cap
	t = (cone->minimum - ray->point.y) / ray->vect.y;
	x = ray->point.x + t * ray->vect.x;
	z = ray->point.z + t * ray->vect.z;
	radius = fabsf(cone->minimum);
	dist = sqrtf(powf(x, 2) + powf(z, 2));
	// printf("  >> Bottom cap: t=%f, point=(%f, %f, %f), radius=%f, dist=%f, hit=%d\n",
	// 	t, x, cone->minimum, z, radius, dist, dist <= radius);
	if (check_cap_cone(ray, t, cone->minimum))
	{
		// printf("  >> Bottom cap HIT added!\n");
		set_cap_value(xs, cone, t);
	}
	
	// Top cap
	t = (cone->maximum - ray->point.y) / ray->vect.y;
	x = ray->point.x + t * ray->vect.x;
	z = ray->point.z + t * ray->vect.z;
	radius = fabsf(cone->maximum);
	dist = sqrtf(powf(x, 2) + powf(z, 2));
	// printf("  >> Top cap: t=%f, point=(%f, %f, %f), radius=%f, dist=%f, hit=%d\n",
	// 	t, x, cone->maximum, z, radius, dist, dist <= radius);
	if (check_cap_cone(ray, t, cone->maximum))
	{
		// printf("  >> Top cap HIT added!\n");
		set_cap_value(xs, cone, t);
	}
	return ;
}