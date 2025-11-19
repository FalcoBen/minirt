#include "../../MiniRt.h"
#include "../../fbenalla/miniRT/parsing.h"

static t_matrix	*get_cone_scale_matrix(float angle_degrees, float height)
{
	float	angle_radians;
	float	scale_xz;

	angle_radians = angle_degrees * M_PI / 180.0f;
	scale_xz = tanf(angle_radians);
	return (scaling(scale_xz, height / 2.0f, scale_xz));
}

static t_matrix	*get_center_offset_translation(t_tuple *axis, \
				float minimum, float maximum, float height)
{
	float	center_offset;
	t_tuple	offset_vec;

	offset_vec = *axis;
	center_offset = (maximum + minimum) / 2.0f;
	s_vec_norm(&offset_vec);
	s_scalar_mult(&offset_vec, offset_vec, center_offset * (height / 2.0f));
	return (translation(offset_vec.x, offset_vec.y, offset_vec.z));
}

static t_matrix	*get_position_translation(t_tuple *position)
{
	return (translation(position->x, position->y, position->z));
}t_matrix *apply_cone_transforms(t_tuple *position, t_tuple *axis,
					t_cone_params *p, t_matrix *temp)
{
	t_matrix	*rotation;
	t_matrix	*scale;
	t_matrix	*center_offset;
	t_matrix	*trans_to_pos;

	rotation = get_axis_alignment_rotation(axis);
	scale = get_cone_scale_matrix(p->angle_degrees, p->height);
	center_offset = get_center_offset_translation(axis,
					p->minimum, p->maximum, p->height);
	trans_to_pos = get_position_translation(position);

	temp = matrix_multi(trans_to_pos, center_offset);
	temp = matrix_multi(temp, rotation);
	temp = matrix_multi(temp, scale);
	copy_matrix_contant(temp);
	return (temp);
}

t_matrix	*create_cone_transform_v2(t_tuple *position, t_tuple *axis,
					t_cone_params *params, t_matrix *pre_allocated)
{
	return (apply_cone_transforms(position, axis, params, pre_allocated));
}