#include "../MiniRt.h"
#include "../../fbenalla/miniRT/parsing.h"

int  is_aligned_with_y(t_tuple *norm_axis)
{
	t_tuple	default_y = {0, 1, 0, 0};
	float	dot;

	dot = dot_product(&default_y, norm_axis);
	dot = fmax(fmin(dot, 1.0f), -1.0f);
	if (fabsf(dot - 1.0f) < 1e-3f)
		return (1);
	if (fabsf(dot + 1.0f) < 1e-3f)
		return (-1);
	return (0);
}

t_matrix *handle_special_cases_cone(int align_code)
{
	if (align_code == 1)
		return (identity_matrix(4, 4));
	if (align_code == -1)
		return (rotation_x(M_PI));
	return (NULL);
}

t_matrix *rotate_to_y(t_tuple *norm_axis)
{
	t_tuple	default_y = {0, 1, 0, 0};
	t_tuple	cross;
	t_tuple	rot_axis;
	float	angle;

	cross = *cross_product(&default_y, norm_axis);
	s_vec_norm(&cross);
	rot_axis = cross;
	angle = acos(dot_product(&default_y, norm_axis));
	return (rotation_axis_angle(&rot_axis, angle));
}

t_matrix	*get_axis_alignment_rotation(t_tuple *axis)
{
	t_tuple	norm_axis;
	int		aligned;

	norm_axis = *axis;
	s_vec_norm(&norm_axis);
	aligned = is_aligned_with_y(&norm_axis);
	if (aligned != 0)
		return (handle_special_cases_cone(aligned));
	return (rotate_to_y(&norm_axis));
}
