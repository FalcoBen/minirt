#include "../../MiniRt.h"


void	sphere_uv(t_tuple *point, t_sphere *sphere, ld *u, ld *v)
{
	t_matrix	point_matrix;
	t_tuple		local_point;
	ld			phi;
	ld			theta;

	point_matrix = s_matrix_tuple(sphere->inv, point);
	local_point = s_matrix_to_tuple(&point_matrix);
	phi = atan2(local_point.z, local_point.x);
	theta = asin(local_point.y);
	*u = 1 - (phi + M_PI) / (2 * M_PI);
	*v = (theta + M_PI / 2) / M_PI;
	if (sphere->material->has_bump_map
		&& sphere->material->bump_map
		&& sphere->material->bump_map->type == 1)
	{
		*u *= 2.0;
		*v *= 2.0;
	}
}


void	cylinder_uv(t_tuple *point, t_cylinder *cylinder, ld *u, ld *v)
{
	t_matrix	point_matrix;
	t_tuple		local_point;
	ld			theta;
	ld			range;

	point_matrix = s_matrix_tuple(cylinder->inv, point);
	local_point = s_matrix_to_tuple(&point_matrix);
	theta = atan2(local_point.x, local_point.z);
	*u = 1.0 - (theta + M_PI) / (2.0 * M_PI);
	if (isinf(cylinder->minimum) || isinf(cylinder->maximum))
	{
		*v = local_point.y * 0.1;
		*v = fmod(*v, 1.0);
		if (*v < 0)
			*v += 1.0;
	}
	else
	{
		range = cylinder->maximum - cylinder->minimum;
		if (fabs(range) > 0.0001)
			*v = (local_point.y - cylinder->minimum) / range;
		else
			*v = 0.5;
	}
	*u = fmod(*u, 1.0);
	if (*u < 0)
		*u += 1.0;
	if (isnan(*v) || isinf(*v))
		*v = 0.0;
}

void	cone_uv(t_tuple *point, t_cone *cone, ld *u, ld *v)
{
	t_matrix	point_matrix;
	t_tuple		local_point;
	ld			theta;
	ld			range;

	point_matrix = s_matrix_tuple(cone->inv, point);
	local_point = s_matrix_to_tuple(&point_matrix);
	theta = atan2(local_point.x, local_point.z);
	*u = 1.0 - (theta + M_PI) / (2.0 * M_PI);
	range = cone->maximum - cone->minimum;
	if (fabs(range) > 0.0001)
		*v = (local_point.y - cone->minimum) / range;
	else
		*v = 0.5;
	*u = fmod(*u, 1.0);
	if (*u < 0)
		*u += 1.0;
	*v = fmax(0.0, fmin(1.0, *v));
}

static void plane_uv_checker(t_tuple *point, t_plane *plane, ld *u, ld *v)
{
	t_matrix 	m;
	t_tuple		loc;
	
	m = s_matrix_tuple(plane->inv, point);
	loc = s_matrix_to_tuple(&m);
	ld scale = 10.0;                                  
	*u = loc.x / scale;
	*v = loc.z / scale;

	*u = fmod(*u, 1.0);
	*v = fmod(*v, 1.0);
	if (*u < 0) 
		*u += 1.0;
	if (*v < 0)
		*v += 1.0;
}


static void plane_uv_image(t_tuple *point, t_plane *plane, ld *u, ld *v)
{
	t_matrix	m;
	t_tuple		loc;
	ld			scale;
	
	m   = s_matrix_tuple(plane->inv, point);
	loc = s_matrix_to_tuple(&m);
	scale = 12.0;                   
	*u = loc.x / scale;
	*v = loc.z / scale;
	*u = *u * 0.5 + 0.5;
	*v = *v * 0.5 + 0.5;
	*u = fmax(0.0, fmin(1.0, *u));
	*v = fmax(0.0, fmin(1.0, *v));
}

void plane_uv(t_tuple *point, t_plane *plane, ld *u, ld *v)
{
	if (plane->material->has_color_texture &&
			plane->material->color_texture &&
			plane->material->color_texture->type == 1)
	{
		plane_uv_checker(point, plane, u, v);
	}
	else if (plane->material->has_bump_map &&
			plane->material->bump_map &&
			plane->material->bump_map->type == 2 &&
			plane->material->bump_map->image)
	{
		plane_uv_image(point, plane, u, v);
	}
	else
	{
		*u = 0.0; *v = 0.0;
	}
}

int number_of_all_objects(t_scene *scene)
{
	int counter = 0;
	t_sphere_fb *sp = scene->sphere;
	t_plane_fb *pl = scene->plane;
	t_cylinder_fb *cy = scene->cylinder;
	t_cone_fb *co = scene->cone;
	while(sp)
	{
		counter++;
		sp = sp->next;
	}
	while(pl)
	{
		counter++;
		pl = pl->next;
	}
	while(cy)
	{
		counter++;
		cy = cy->next;
	}
	while(co)
	{
		counter++;
		co = co->next;
	}
	return counter;
}
