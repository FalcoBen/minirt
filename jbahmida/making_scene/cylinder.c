#include "../../MiniRt.h"

void	cylinder(t_cylinder **cylinder, t_matrix *trans, bool flag)
{
	static int	id;

	if (!cylinder)
		return ;
	if (flag)
	{
		(*cylinder)->transform = trans;
		(*cylinder)->inv = matrix_inverte((*cylinder)->transform);
		return ;
	}
	id = 0;
	(*cylinder) = alloc(sizeof(t_cylinder), false);
	(*cylinder)->transform = identity_matrix(4, 4);
	(*cylinder)->inv = identity_matrix(4, 4);
	(*cylinder)->minimum = -INFINITY;
	(*cylinder)->maximum = INFINITY;
	(*cylinder)->id = id;
	(*cylinder)->material = material();
	(*cylinder)->closed = false;
	id++;
}

void	skew_symmetric(t_tuple *axis, t_matrix *result)
{
	result->matrix[0][0] = 0;
	result->matrix[0][1] = -axis->z;
	result->matrix[0][2] = axis->y;
	result->matrix[0][3] = 0;
	result->matrix[1][0] = axis->z;
	result->matrix[1][1] = 0;
	result->matrix[1][2] = -axis->x;
	result->matrix[1][3] = 0;
	result->matrix[2][0] = -axis->y;
	result->matrix[2][1] = axis->x;
	result->matrix[2][2] = 0;
	result->matrix[2][3] = 0;
	result->matrix[3][0] = 0;
	result->matrix[3][1] = 0;
	result->matrix[3][2] = 0;
	result->matrix[3][3] = 1;
}

t_matrix	*rotation_axis_angle(t_tuple *axis, ld angle)
{
	t_matrix	*rot;
	t_tuple		norm_axis;
	t_matrix	*skew;
	ld			cos_theta;
	ld			sin_theta;
	int     i;
    int     j;
    ld      outer;
    ld      diag_part;

	rot = identity_matrix(4, 4);
	norm_axis = *axis;
	s_vec_norm(&norm_axis);
	skew = matrix_constractor(4,4,(ld[16]){0}, 16);
	skew_symmetric(&norm_axis, skew);
	cos_theta = cos(angle);
	sin_theta = sin(angle);


	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			if (i == j)
				diag_part = cos_theta;
			else
				diag_part = 0.0;
			outer = (1.0 - cos_theta)
				* (norm_axis.x * norm_axis.x * (i == 0)
					+ norm_axis.y * norm_axis.y * (i == 1)
					+ norm_axis.z * norm_axis.z * (i == 2));
			rot->matrix[i][j] = diag_part
				+ outer
				+ skew->matrix[i][j] * sin_theta;
			j++;
		}
		i++;
	}
	return (rot);
}


t_matrix	*create_cylinder_transform(t_tuple *position, t_tuple *axis,
		ld radius, ld height)
{
	t_tuple		normalized_axis;
	t_tuple		default_y;
	t_matrix	*rotation;
	t_tuple		rot_axis;
	t_matrix	*scale;
	t_matrix	*trans;
	t_matrix	*temp;
	t_matrix	*result;
	ld			dot;
	ld			angle;

	normalized_axis = *axis;
	s_vec_norm(&normalized_axis);
	default_y = (t_tuple){0, 1, 0, 0};
	dot = dot_product(&default_y, &normalized_axis);
	dot = fmax(fmin(dot, 1.0), -1.0);
	rotation = NULL;
	if (fabs(dot - 1.0) < 1e-3)
		rotation = identity_matrix(4, 4);
	else if (fabs(dot + 1.0) < 1e-3)
		rotation = rotation_x(M_PI);
	else
	{
		rot_axis = *cross_product(&default_y, &normalized_axis);
		s_vec_norm(&rot_axis);
		angle = acos(dot);
		rotation = rotation_axis_angle(&rot_axis, angle);
	}

	scale = scaling(radius, height / 2.0, radius);
	trans = translation(position->x, position->y, position->z);

	temp = matrix_multi(trans, rotation);
	result = matrix_multi(temp, scale);
	copy_matrix_contant(result);
	return (result);
}


bool	check_cap_cylinder(t_stack_ray *ray, ld t)
{
	ld	x;
	ld	z;

	x = ray->point.x + t * ray->vect.x;
	z = ray->point.z + t * ray->vect.z;
	return ((powf(x, 2) + powf(z, 2)) <= 1);
}

void	intersect_caps_cylinder(t_cylinder *cylinder,
	t_stack_ray *ray, t_stack_intersections *xs)
{
	ld	t;

	if (!cylinder->closed || ldbl_cmp(ray->vect.y, 0.0))
		return ;
	t = (cylinder->minimum - ray->point.y) / ray->vect.y;
	if (check_cap_cylinder(ray, t))
	{
		xs->inters_list[xs->count].inters_value = t;
		xs->inters_list[xs->count].sphere = NULL;
		xs->inters_list[xs->count].plane = NULL;
		xs->inters_list[xs->count].cylinder = cylinder;
		xs->inters_list[xs->count].cone = NULL;
		xs->inters_list[xs->count].existence = true;
		xs->inters_list[xs->count].type = T_CYLINDRE;
		xs->count++;
	}

	t = (cylinder->maximum - ray->point.y) / ray->vect.y;
	if (check_cap_cylinder(ray, t))
	{
		xs->inters_list[xs->count].inters_value = t;
		xs->inters_list[xs->count].sphere = NULL;
		xs->inters_list[xs->count].plane = NULL;
		xs->inters_list[xs->count].cylinder = cylinder;
		xs->inters_list[xs->count].cone = NULL;
		xs->inters_list[xs->count].existence = true;
		xs->inters_list[xs->count].type = T_CYLINDRE;
		xs->count++;
	}
}

void	cylinder_intersct(t_stack_intersections *xs, t_cylinder *cylinder, t_stack_ray *ray)
{
	t_stack_ray	local_ray;
	ld			a;
	ld			b;
	ld			c;
	ld			delta;
	ld			t0;
	ld			t1;
	ld			y0;
	ld			y1;
	ld			tmp;

	local_ray = s_transform(ray, cylinder->inv);
	a = powf(local_ray.vect.x, 2) + powf(local_ray.vect.z, 2);
	xs->count = 0;
	xs->existence = false;
	if (!ldbl_cmp(a, 0.0))
	{
		b = (2 * local_ray.point.x * local_ray.vect.x)
			+ (2 * local_ray.point.z * local_ray.vect.z);
		c = powf(local_ray.point.x, 2) + powf(local_ray.point.z, 2) - 1;
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
			if (y0 > cylinder->minimum && y0 < cylinder->maximum)
			{
				xs->inters_list[xs->count].inters_value = t0;
				xs->inters_list[xs->count].sphere = NULL;
				xs->inters_list[xs->count].plane = NULL;
				xs->inters_list[xs->count].cylinder = cylinder;
				xs->inters_list[xs->count].cone = NULL;
				xs->inters_list[xs->count].existence = true;
				xs->inters_list[xs->count].type = T_CYLINDRE;
				xs->count++;
			}
			y1 = local_ray.point.y + t1 * local_ray.vect.y;
			if (y1 > cylinder->minimum && y1 < cylinder->maximum)
			{
				xs->inters_list[xs->count].inters_value = t1;
				xs->inters_list[xs->count].sphere = NULL;
				xs->inters_list[xs->count].plane = NULL;
				xs->inters_list[xs->count].cylinder = cylinder;
				xs->inters_list[xs->count].cone = NULL;
				xs->inters_list[xs->count].existence = true;
				xs->inters_list[xs->count].type = T_CYLINDRE;
				xs->count++;
			}
		}
	}
	intersect_caps_cylinder(cylinder, &local_ray, xs);
	xs->existence = xs->count > 0;
	if (!xs->existence)
		xs->world_inters_list = NULL;
}

void	s_cylinder_normal(t_tuple *norm, t_cylinder *cylinder, t_tuple *point)
{
	t_matrix	point_matrix;
	t_tuple		local_point;
	t_tuple		local_normal;
	ld			dist;
	t_matrix	transpose;
	t_matrix	world_normal_matrix;

	point_matrix = s_matrix_tuple(cylinder->inv, point);
	local_point = s_matrix_to_tuple(&point_matrix);
	dist = powf(local_point.x, 2) + powf(local_point.z, 2);
	if (cylinder->closed && dist < 1
		&& local_point.y >= cylinder->maximum - FLT_EPSILON)
		local_normal = (t_tuple){0, 1, 0, 0};
	else if (cylinder->closed && dist < 1
		&& local_point.y <= cylinder->minimum + FLT_EPSILON)
		local_normal = (t_tuple){0, -1, 0, 0};
	else
		local_normal = (t_tuple){local_point.x, 0, local_point.z, 0};
	transpose = s_matrix_transpos(cylinder->inv);
	world_normal_matrix = s_matrix_tuple(&transpose, &local_normal);
	*norm = s_matrix_to_tuple(&world_normal_matrix);
	norm->w = 0;
	s_vec_norm(norm);
}


