/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniRt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 22:12:40 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/16 22:12:48 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H


# include <math.h>
# include <float.h>

# ifndef  M_PI
#  define M_PI 3.1415926535897932384626433
# endif
# define SHADOW_EPSILON 0.00020f

# define INTER_EPSILON 1e-6
# include <stdio.h>
# include <stdlib.h>
// # include <string.h>
# include <stdbool.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include "MLX42.h"
# include "garbage_collector/my_malloc.h"
# include <sys/resource.h>
# include "fbenalla/miniRT/parsing.h"

// # define WIDTH 4480
// # define HEIGHT 2520
# define WIDTH 1300
# define HEIGHT 1000
// # define WIDTH 2200
// # define HEIGHT 1800
// #define WIDTH 100
// #define HEIGHT 100

// # define ld float

typedef struct s_tupels			t_tupels;
typedef struct s_color			t_color;
typedef enum e_type				t_type;
typedef struct s_sphere			t_sphere;
typedef union s_obj				t_obj;
typedef struct s_object			t_object;
typedef struct s_light			t_light;
typedef struct s_world			t_world;
typedef struct s_canvas			t_canvas;
typedef struct s_matrix			t_matrix;
typedef struct s_shear_param	t_shear_param;
typedef struct s_material		t_material;
typedef struct s_ray			t_ray;
typedef struct s_inters			t_inters;
typedef struct s_intersections	t_intersections;
typedef struct s_computation	t_computation;
typedef struct s_camera			t_camera;

typedef struct s_tupels
{
	float	x;
	float	y;
	float	z;
	float	w;
}	t_tuple;

typedef struct s_color
{
	float	r;
	float	g;
	float	b;
}	t_color;

typedef struct s_plane
{
	int			id;
	t_tuple		*origin;
	t_tuple		*vect;
	t_matrix	*transform;
	t_matrix	*inv;
	t_material	*material;
}	t_plane;

typedef struct s_sphere
{
	int			id;
	t_tuple		*origin;
	float		radius;
	t_matrix	*transform;
	t_matrix	*inv;
	t_material	*material;
}	t_sphere;

typedef struct s_cylinder
{
	int			id;
	t_tuple		*origin;
	float		radius;
	float		minimum;
	float		maximum;
	t_matrix	*transform;
	t_matrix	*inv;
	t_material	*material;
	bool		closed;
}	t_cylinder;

typedef struct s_cone
{
	int			id;
	t_tuple		*origin;
	float		minimum;
	float		maximum;
	t_matrix	*transform;
	t_matrix	*inv;
	t_material	*material;
	bool		closed;
    float       angle;
}	t_cone;

typedef union s_obj
{
	t_sphere	*sphere;
	t_cylinder	*cylinder;
	t_cone		*cone;
	t_plane		*plane;
}	t_obj;

typedef struct s_object
{
	t_type	type;
	t_obj	*obj;
}	t_object;

typedef struct s_light
{
	t_color	*intensity;
	t_tuple	*position;
}	t_light;

typedef struct s_world
{
	int			obj_num;
	int			light_num;
	t_light		**lights;
	t_object	**objects;
}	t_world;

typedef struct s_canvas
{
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_world		*world;
	t_camera	*cam;
}	t_canva;

typedef struct s_matrix
{
	int		row;
	int		col;
	float	**matrix;
	float	stack_matrix[4][4];
}	t_matrix;

typedef struct s_shear_param
{
	float	x_to_y;
	float	x_to_z;
	float	y_to_x;
	float	y_to_z;
	float	z_to_x;
	float	z_to_y;
}	t_shear;

typedef struct s_material
{
	t_color		*color;
	float		ambient;
	float		diffuse;
	float		specular;
	float		shininess;
	bool		has_color_texture;
	t_texture	*color_texture;
	bool		has_bump_map;
	t_texture	*bump_map;
	double		bump_strength;
}	t_material;

typedef struct s_ray
{
	t_tuple	point;
	t_tuple	vect;
}	t_ray;

typedef struct stack_ray
{
	t_tuple	point;
	t_tuple	vect;
}	t_stack_ray;

typedef struct s_inters
{
	t_type		type;
	t_plane		*plane;
	t_sphere	*sphere;
	t_cylinder	*cylinder;
	t_cone		*cone;
	float		inters_value;
	bool		existence;
}	t_inters;

typedef struct s_intersections
{
	bool		existence;
	int			count;
	t_inters	**inters_list;
}	t_intersections;

typedef struct s_stack_intersections
{
	bool		existence;
	int			count;
	t_inters	inters_list[8];
	t_inters	*world_inters_list;
}	t_stack_intersections;

typedef struct s_computation
{
	void		*obj;
	t_type		obj_type;
	t_tuple		point;
	t_tuple		eyev;
	t_tuple		normalv;
	t_tuple		over_point;
	t_inters	*intersection;
	float		t;
	bool		inside;
}	t_comp;

typedef struct s_camera
{
	float		hsize;
	float		vsize;
	float		field_ov;
	float		pixel_size;
	float		aspect;
	float		half_width;
	float		half_height;
	t_matrix	*tran;
	t_matrix	*tran_inv;
}t_camera;

typedef struct s_norminette_cam
{
	t_tuple		cam_pos;
	t_tuple		cam_dir;
	t_tuple		to;
	t_tuple		up;
	float		length;
	float		fov_radians;
	t_matrix	tran;
}	t_ncam;

typedef struct s_nor_write_pixel
{
	t_canva		*canva;
	t_stack_ray	ray;
	t_color		color;
	int			a;
	float		x;
	float		y;
}	t_nwrite_pixel;

typedef struct s_nor_ray_for_camera
{
	float		xoffset;
	float		yoffset;
	float		world_x;
	float		world_y;
	t_tuple		t;
	t_tuple		t1;
	t_matrix	tmp;
	t_matrix	tmp1;
	t_tuple		pixel;
	t_tuple		origin;
	t_tuple		direction;
}	t_nor_ray_for_camera;

typedef struct n_nor_intersect
{
	float		a;
	float		b;
	float		c;
	float		delta;
	float		sqrt_delta;
	t_stack_ray	local_ray;
}t_nor_intersect;

typedef struct n_v_tran
{
	t_tuple		forward;
	t_tuple		upn;
	t_tuple		left;
	t_tuple		true_up;
	t_matrix	tmp;
}t_v_tran;

typedef struct n_nor_shade_hit
{
	t_color		final_color;
	t_material	*material;
	t_material	temp_material;
	t_color		base_color;
	t_color		light_contribution;
	bool		shadowed;
	int			i;
}t_nor_shade_hit;

typedef struct n_roaa
{
	t_tuple		norm_axis;
	t_matrix	*skew;
	float		cos_theta;
	float		sin_theta;
	float		outer;
	float		diag_part;
}t_roaa;

typedef struct n_nor_cy_tran
{
	t_tuple		normalized_axis;
	t_tuple		default_y;
	t_matrix	*rotation;
	t_tuple		rot_axis;
	t_matrix	*scale;
	t_matrix	*trans;
	t_matrix	*temp;
	t_matrix	*result;
	float		angle;
	float		dot;

}t_nor_cy_tran;

typedef struct n_nor_cy_nor
{
	t_matrix	point_matrix;
	t_tuple		local_point;
	t_tuple		local_normal;
	float		dist;
	t_matrix	transpose;
	t_matrix	world_normal_matrix;
}t_nor_cy_nor;

typedef struct n_nor_cy_inter
{
	t_stack_ray	local_ray;
	float		a;
	float		b;
	float		c;
	float		delta;
	float		t0;
	float		t1;
	float		y0;
	float		y1;
	float		tmp;
}t_nor_cy_inter;

typedef struct n_nor_cone_inter
{
	t_stack_ray	local_ray;
	float		a;
	float		b;
	float		c;
	float		delta;
	float		t;
	float		t0;
	float		t1;
	float		tmp;
	float		y;
	float		y0;
	float		y1;
}t_nor_cone_inter;

typedef struct n_nor_roft
{
	t_tuple		v;
	float		factor;
	float		s;
	float		c;
	float		vx[9];
	t_matrix	*result;
}t_nor_roft;

typedef struct n_nor_cone_tran
{
	t_tuple		normalized_axis;
	t_tuple		rot_axis;
	t_tuple		default_y;
	t_matrix	*rotation;
	t_matrix	*scale;
	t_matrix	*trans;
	t_matrix	*temp;
	t_matrix	*result;
		float		dot;
	float		angle;
}	t_nor_cone_tran;

typedef struct n_nor_lighting
{
	t_color	ambient;
	t_color	diffuse;
	t_color	specular;
	t_color	effective_color;
	t_tuple	*lightv;
	t_tuple	*reflectv;
	t_color	*result;
	float	light_dot_normal;
	float	reflect_dot_eye;
	float	factor;
}t_nor_lighting;

typedef struct n_nor_s_lighting
{
	t_color		effective_color;
	t_tuple		lightv;
	t_material	material;
	t_light		light;
	t_color		ambient;
	t_color		diffuse;
	t_color		specular;
	t_color		tmp;
	t_tuple		normalv;
	t_tuple		eyev;
}	t_nor_s_lighting;

typedef struct s_submatrix_data
{
	t_matrix	*m;
	int			r;
	int			c;
	float		*va_list;
	int			va_size;
}	t_submatrix_data;

typedef struct s_cone_params
{
	float	angle_degrees;
	float	height;
	float	minimum;
	float	maximum;
}	t_cone_params;

typedef struct s_matrix_mult_data
{
	t_matrix	*m1;
	t_matrix	*m2;
	float		*va_list;
	int			v_size;
}	t_matrix_mult_data;

void			write_pixel(void *c);
t_tuple			get_up_vector(t_tuple *orientation);
void			close_window(mlx_key_data_t keydata, void *param);
t_camera		*create_camera(t_scene *scene);
void			jassim_mlx(t_scene *scene);
int				combine_color(t_color color);
t_canva			s_canva_create(mlx_t *p, int w, int h);
t_color			color_at(t_world *w, t_stack_ray *ray);
t_color			s_create_color(float *vals);
void			copy_matrix_contant(t_matrix *m);
void			s_intersect_world(t_stack_intersections *in,
					t_world world, t_stack_ray ray);
void			s_intersect_concatenate(t_stack_intersections *arr1,
					t_stack_intersections *arr2);
void			sort_intersctions(t_stack_intersections *inters);
t_tuple			*create_vector(float *coord);
t_tuple			*create_point(float *coord);
t_tuple			*tuple_constractor(float x, float y,
					float z, float w);
bool			cmp_tuple(t_tuple *t1, t_tuple *t2);
t_tuple			*add_t(t_tuple *t1, t_tuple *t2);
t_tuple			*sub_t(t_tuple *t1, t_tuple *t2);
t_tuple			*neg_t(t_tuple *t);
t_tuple			*scalar_mult(t_tuple *t, float scalar);
float			magn(t_tuple *t);
t_tuple			*vec_norm(t_tuple *t);
float			dot_product(t_tuple *t1, t_tuple *t2);
t_tuple			*cross_product(t_tuple *t1, t_tuple *t2);
bool			ldbl_cmp(float a, float b);
/*****************************************************/
t_matrix		*matrix_constractor(int row, int col, float *arr, size_t arr_s);
t_matrix		*identity_matrix(int row, int col);
bool			cmp_matrix(t_matrix *m1, t_matrix *m2);
t_matrix		*matrix_multi(t_matrix *m1, t_matrix *m2);
t_matrix		*matrix_tuple(t_matrix *m, t_tuple *t);
t_matrix		*matrix_transpos(t_matrix *m);
t_matrix		*matrix_inverte(t_matrix *m);
float			matrix_determinant(t_matrix *m);
float			matrix_cofactor(t_matrix *m, int row, int col);
t_matrix		*submatrix(t_matrix *m, int r, int c);
float			det_small_matrix(t_matrix *m);
t_tuple			*matrix_to_tuple(t_matrix *m);
/*****************************************************/
t_matrix		*translation(float x, float y, float z);
t_matrix		*scaling(float x, float y, float z);
t_matrix		*rotation_x(float angle);
t_matrix		*rotation_y(float angle);
t_matrix		*rotation_z(float angle);
t_matrix		*shearing(t_shear *param);
/*****************************************************/
void			sphere(t_sphere **sphere, t_matrix *tran, bool flag);
t_ray			*ray_constrctor(t_tuple *origin, t_tuple *vect);
t_tuple			*position(t_ray *ray, float t);
t_inters		*intersection(float p, t_sphere *obj);
t_intersections	*intersections(int count, t_inters **list_objs);
t_ray			*transform(t_ray *ray, t_matrix *m);
t_intersections	*intersect(t_ray *ray, t_sphere *s);
t_inters		*hit(t_intersections *inters);
void			set_transform(t_sphere **s, t_matrix *transformation);
/*****************************************************/
t_material		*material(void);
t_light			*point_light(t_color *color, t_tuple *position);
t_tuple			*normal_at(t_sphere *s, t_tuple *point);
t_tuple			*reflect(t_tuple *in, t_tuple *normal);
t_color			*hadamard(t_color *c1, t_color *c2);
t_color			*color_scalar(t_color *c, float s);
t_color			*color_add(t_color *c1, t_color *c2);
t_color			*split_color(int c);
t_color			*create_color(float *vals);
t_color			s_hadamard(t_color *c1, t_color *c2);
t_color			s_color_scalar(t_color *c, float s);
t_color			s_color_add(t_color *c1, t_color *c2);
t_tuple			s_reflect(t_tuple in, t_tuple normal);
float			tone_map(float x);
float			gamma_correct(float x, float gamma);
/*****************************************************/
// t_matrix *view_transformation(t_tuple *from, t_tuple *to, t_tuple *up);
/*****************************************************/
/*****************************************************/
t_camera		*camera(float hsize, float vsize, float field_ov, t_matrix *m);
t_ray			*ray_for_camera(t_camera *cam, float x, float y);
t_canva			*create_canvas(mlx_t *p, unsigned long width,
					unsigned long height);
t_tuple			s_create_tuple(float *coord, int type);
t_stack_ray		s_ray_constrctor(t_tuple *origin, t_tuple *vect);
t_stack_ray		s_transform(t_stack_ray *ray, t_matrix *m);
t_tuple			s_matrix_to_tuple(t_matrix *m);
t_tuple			s_matrix_to_tuple1(t_matrix *m);
t_tuple			s_position(t_stack_ray *ray, float t);
t_matrix		s_matrix_tuple(t_matrix *m, t_tuple *t);
t_inters		s_hit(t_stack_intersections inters);
void			s_intersect(t_stack_intersections *xs,
					t_stack_ray ray, t_sphere *s);
t_matrix		s_matrix_constractor(int row, int col, float *arr);
t_matrix		s_matrix_multi(t_matrix *m1, t_matrix *m2);
void			s_add_t(t_tuple *res, t_tuple t1, t_tuple t2);
void			s_scalar_mult(t_tuple *res, t_tuple t, float scalar);
void			s_normal_at(t_tuple *normal, t_sphere *s, t_tuple *point);
void			s_vec_norm(t_tuple *t);
t_matrix		s_matrix_transpos(t_matrix *m);
void			s_scalar_mult(t_tuple *res, t_tuple t, float scalar);
void			s_sub_t(t_tuple *res, t_tuple t1, t_tuple t2);
void			s_add_t(t_tuple *res, t_tuple t1, t_tuple t2);
t_tuple			s_neg_t(t_tuple t);
void			s_neg_t1(t_tuple *t);
t_color			s_lighting(t_world *world, t_comp *comp, t_nor_shade_hit var);
void			copy_matrix_contant(t_matrix *m);
t_comp			prepare_computations(t_inters *intersection, t_stack_ray *ray);
t_color			shade_hit(t_world *worfloat, t_comp *comp);
t_color			color_at(t_world *w, t_stack_ray *ray);
t_matrix		view_transformation(t_tuple *from, t_tuple *to, t_tuple *up);
t_stack_ray		s_ray_for_camera(t_camera *cam, float x, float y);
void			copy_matrix_contant(t_matrix *m);
void			s_intersect_world(t_stack_intersections *in,
					t_world world, t_stack_ray ray);
void			s_intersect_concatenate(t_stack_intersections *arr1,
					t_stack_intersections *arr2);
bool			is_shadowed(t_world *world, t_tuple point, t_light *light);
void			cone(t_cone **cone, t_matrix *trans, bool flag);
t_matrix		*rotation_from_to(t_tuple *from, t_tuple *to);
void			cone_intersct(t_stack_intersections *xs,
					t_cone *cone, t_stack_ray *ray);
void			s_cone_normal(t_tuple *norm, t_cone *cone, t_tuple *point);
void			intersect_caps_cone(t_cone *cone,
					t_stack_ray *ray, t_stack_intersections *xs);
void			cylinder(t_cylinder **cylinder, t_matrix *trans, bool flag);
bool			check_cap_cylinder(t_stack_ray *ray, float t);
void			intersect_caps_cylinder(t_cylinder *cylinder,
					t_stack_ray *ray, t_stack_intersections *xs);
void			cylinder_intersct(t_stack_intersections *xs,
					t_cylinder *cylinder, t_stack_ray *ray);
void			s_cylinder_normal(t_tuple *norm,
					t_cylinder *cylinder, t_tuple *point);
void			plane(t_plane **plane, t_tuple *origin, t_tuple *vect);
void			plane_intersct(t_stack_intersections *xs,
					t_plane *plane, t_stack_ray *ray);
void			s_plane_normal(t_tuple *norm, t_plane *plane, t_tuple *point);
t_color			shade_hit_with_ambient(t_world *world, t_comp *comp);
// t_matrix		*create_cone_transform(t_tuple *position, t_tuple *axis,
// 					float radius, float height);
t_matrix    *create_cone_transform_v2(t_tuple *position, t_tuple *axis,
                    t_cone_params *params, t_matrix *pre_allocated);
t_matrix		*create_cylinder_transform(t_tuple *position, t_tuple *axis,
					float radius, float height);
t_matrix		*create_plane_transform(t_tuple *origin, t_tuple *normal);
t_world			s_world(t_scene *scene);
/*-----------------------------------------------------------*/
void			parsing(int fd);
t_scene			*linking_fucntion(t_scene *scene, bool set);
void			set_scene(t_scene *scene, bool flag);
void			jassim_mlx(t_scene *scene);
void			obj_creator_sphere(t_obj *obj, t_sphere_fb *src,
					t_ambient_light_fb *ambient_light);
void			obj_creator_plane(t_obj *obj, t_plane_fb *src,
					t_ambient_light_fb *ambient_light);
void			close_window(mlx_key_data_t keydata, void *param);
int				number_of_all_objects(t_scene *scene);
// fahd
// Texture functions
void			sphere_uv(t_tuple *point, t_sphere *sphere, float *u, float *v);
void			plane_uv(t_tuple *point, t_plane *plane, float *u, float *v);
void			cylinder_uv(t_tuple *point, t_cylinder *cylinder, \
				float *u, float *v);
void			cone_uv(t_tuple *point, t_cone *cone, float *u, float *v);
// Your existing texture functions (from parsing.h)
double			sample_height(t_texture *tex, double u, double v);
t_texture_color	sample_color(t_texture *tex, double u, double v);
t_tuple			*cross_product(t_tuple *t1, t_tuple *t2);
t_tuple			s_cross_product(t_tuple *t1, t_tuple *t2);
void			plane_uv_with_dimensions(t_tuple *point,
					t_plane *plane, float *u, float *v);
void			plane_uv_autodetect(t_tuple *point,
					t_plane *plane, float *u, float *v);
void			plane_uv_simple_fix(t_tuple *point,
					t_plane *plane, float *u, float *v);
void			s_plane_normal_bonus(t_tuple *local_normal,
					t_plane *plane, t_tuple *point);
void			s_cylinder_normal_bonus(t_tuple *local_normal,
					t_cylinder *cylinder, t_tuple *point);
void			s_world_cone_constractor(t_world *world,
					t_scene *scene, int *i);
void			s_world_plane_constractor(t_world *world,
					t_scene *scene, int *i);
void			s_world_cylinder_constractor(t_world *world,
					t_scene *scene, int *i);
void			s_world_sphere_constractor(t_world *world,
					t_scene *scene, int *i);
t_color			shade_helper(t_material *material, t_comp *comp);
void			get_uv_coors(t_comp *comp, float *u, float *v);
int				number_of_all_objects(t_scene *scene);
void			plane_uv_checker(t_tuple *point, t_plane *plane, \
				float *u, float *v);
void			plane_uv_image(t_tuple *point, t_plane *plane, \
				float *u, float *v);
void			parsing_second_part(char ***tokens, int *counter, \
					t_cleanup *cleaner);
void			init_input_data(t_objects_fb *input_data);
void			init_cleaner(t_cleanup *cleaner);
void			start_asigning_objects(t_scene *scene, \
					t_objects_fb *dispatch_table, char ***tokens, int counter);
t_matrix		*apply_cone_transforms(t_tuple *position, t_tuple *axis,
                    t_cone_params *p, t_matrix *temp);
t_matrix		*rotation_axis_angle(t_tuple *axis, float angle);
void			check_against_null(t_canva *canva, t_scene *scene);
bool			check_sphere_object(t_object *obj);
bool			check_camera_and_world(t_camera *cam, t_world *world);
bool			check_lights(t_world *world);
bool			check_plane_object(t_object *obj);
bool			check_cylinder_object(t_object *obj);
t_tuple 		cap_normal_max(t_cone *cone, t_tuple *p, float dist_sq);
t_tuple 		cap_normal_min(t_cone *cone, t_tuple *p, float dist_sq);
t_tuple 		side_normal(t_tuple *p, float radius);
int 			is_aligned_with_y(t_tuple *norm_axis);
t_matrix		*handle_special_cases_cone(int align_code);
t_matrix		*rotate_to_y(t_tuple *norm_axis);
t_matrix		*get_axis_alignment_rotation(t_tuple *axis);
// void			clean_textures(t_scene *scene, t_canva *canva, mlx_t *mlx);

#endif