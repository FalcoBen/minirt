#ifndef MINIRT_H
#define MINIRT_H
extern int g_fd;

#include <math.h>
#include <float.h>

#ifndef  M_PI
#define  M_PI  3.1415926535897932384626433
#endif
#define SHADOW_EPSILON 0.00020f
// #define SHADOW_EPSILON 0.0001 
 #define INTER_EPSILON 1e-6
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "MLX42/include/MLX42/MLX42.h"
#include "garbage_collector/my_malloc.h"

#include <sys/resource.h>
#include "fbenalla/miniRT/parsing.h"
// static void tracker(const char *file, int line) {
//     printf("alloc called from %s:%d\n", file, line);
// }
// #define alloc(size, b) (tracker(__FILE__, __LINE__), alloc(size, b))





#define BPP sizeof(int32_t)
#define WIDTH 900
#define HEIGHT 900

#define DW 500
#define DH 500
// #define WIDTH 100
// #define HEIGHT 100

# define  ld float

typedef struct s_tupels t_tupels;
typedef struct s_color t_color;
typedef enum e_type t_type;
typedef struct s_sphere t_sphere;
typedef union s_obj t_obj;
typedef struct s_object t_object;
typedef struct s_light t_light;
typedef struct s_world t_world;
typedef struct s_canvas t_canvas;
typedef struct s_matrix t_matrix;
typedef struct s_shear_param t_shear_param;
typedef struct s_material t_material;
typedef struct s_ray t_ray;
typedef struct s_inters t_inters;
typedef struct s_intersections t_intersections;
typedef struct s_computation t_computation;
typedef struct s_camera t_camera;
typedef struct s_tupels {
    ld x;
    ld y;
    ld z;
    ld w;
}t_tuple;
typedef struct s_color 
{
    ld r;
    ld g;
    ld b;
} t_color;
// typedef enum e_type
// {
//     T_SPHERE,
//     T_CYLINDRE,
//     T_CONE,
//     T_PLAN
// }t_type;


typedef struct s_plane
{
    int id;
    t_tuple *origin;
    t_tuple *vect;
    t_matrix *transform;
    t_matrix *inv;
    t_material *material;
} t_plane;

typedef struct s_sphere
{

    int id;
    t_tuple *origin;
    ld radius;
    t_matrix *transform;
    t_matrix *inv;
    t_material *material;
}t_sphere;

typedef struct s_cylinder
{
    int id;
    t_tuple *origin;
    ld radius;              
    ld minimum;             
    ld maximum;             
    t_matrix *transform;    
    t_matrix *inv;          
    t_material *material;
    bool closed;
} t_cylinder;

typedef struct s_cone
{
    int id;
    t_tuple *origin;
    ld minimum;             
    ld maximum;             
    t_matrix *transform;    
    t_matrix *inv;          
    t_material *material;
    bool closed;
} t_cone;

typedef union s_obj
{
    t_sphere *sphere;
    t_cylinder *cylinder;
    t_cone *cone;
    t_plane *plane;
}t_obj;



typedef struct s_object
{
	t_type type;
	t_obj *obj;
}t_object;
typedef struct s_light
{
    t_color *intensity;
    t_tuple *position;
}t_light;



typedef struct s_world
{
	int obj_num;
    int light_num;
    t_light **lights;
	t_object **objects;
}t_world;



typedef struct s_canvas {
    mlx_t       *mlx;
    mlx_image_t *image;
    t_world *world;
    t_camera *cam;
} t_canva;

typedef struct s_matrix {
    int row;
    int col;
    ld **matrix;
    ld stack_matrix[4][4];
}t_matrix;

typedef struct s_shear_param
{
    ld x_to_y;
    ld x_to_z;
    ld y_to_x;
    ld y_to_z;
    ld z_to_x;
    ld z_to_y;
}t_shear;

typedef struct s_material
{
    t_color *color;
    ld ambient;
    ld diffuse;
    ld specular;
    ld shininess;


    bool has_color_texture;
    t_texture *color_texture;

    bool has_bump_map;
    t_texture *bump_map;
    double bump_strength;
}t_material;



typedef struct s_ray
{
    t_tuple point;
    t_tuple vect;
}t_ray;

typedef struct stack_ray
{
    t_tuple point;
    t_tuple vect;
}t_stack_ray;

typedef struct s_inters
{
    t_type type;
    t_plane *plane;
    t_sphere *sphere;
    t_cylinder *cylinder;
    t_cone *cone;
    ld inters_value;
    bool existence;
}t_inters;

typedef struct s_intersections
{
    bool existence;
    int count;
    t_inters **inters_list;
}t_intersections;

typedef struct s_stack_intersections
{
    bool existence;
    int count;
    t_inters inters_list[8];
    t_inters *world_inters_list;
}t_stack_intersections;





// char mode_switch(int t);





// typedef struct s_computation
// {
//     t_sphere *obj;
//     t_tuple *point;
//     t_tuple *eyev;
//     t_tuple *normalv;
//     t_inters *intersection;
//     bool inside;
// }t_comp;

// typedef struct s_computation
// {
//     t_sphere *obj;
//     t_tuple point;
//     t_tuple eyev;
//     t_tuple normalv;
//     t_tuple over_point;
//     t_inters *intersection;
//     ld t;
//     bool inside;
// }t_comp;

typedef struct s_computation
{
    void *obj;
    t_type obj_type;
    t_tuple point;
    t_tuple eyev;
    t_tuple normalv;
    t_tuple over_point;
    t_inters *intersection;
    ld t;
    bool inside;
} t_comp;

typedef struct s_camera
{
    ld hsize;
    ld vsize;
    ld field_ov;
    ld pixel_size;
    ld aspect;
    ld half_width;
    ld half_height;
    t_matrix *tran;
    t_matrix *tran_inv;
}t_camera;




typedef struct s_norminette_cam
{
    t_tuple cam_pos;
	t_tuple cam_dir;
	t_tuple to;
	t_tuple up;
	ld length;
    ld fov_radians;
	t_matrix tran;
}t_ncam;

typedef struct s_nor_write_pixel
{
    t_canva *canva;
	t_stack_ray ray;
	t_color color;
	int a;
	ld x;
	ld y;
}t_nwrite_pixel;

typedef struct s_nor_ray_for_camera
{
    ld	xoffset;
	ld	yoffset;
	ld	world_x;
	ld	world_y;
	t_tuple t;
	t_tuple t1;
	t_matrix tmp;
	t_matrix tmp1;
	t_tuple pixel;
	t_tuple origin;
	t_tuple direction;
}t_nor_ray_for_camera;

typedef struct  n_nor_intersect
{
    ld          a;
	ld			b;
	ld			c;
	ld			delta;
	ld			sqrt_delta;
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


void write_pixel(void *c);
t_tuple get_up_vector(t_tuple *orientation);
void    close_window(mlx_key_data_t keydata, void* param);
t_camera *create_camera(t_scene *scene);
void jassim_mlx(t_scene *scene);
int combine_color(t_color color);
t_canva s_canva_create(mlx_t *p, int w, int h);
t_color color_at(t_world *w, t_stack_ray *ray);
t_color s_create_color(ld *vals);
void copy_matrix_contant(t_matrix *m);
void s_intersect_world(t_stack_intersections *in, t_world world, t_stack_ray ray);
void s_intersect_concatenate(t_stack_intersections *arr1, t_stack_intersections *arr2);
void sort_intersctions(t_stack_intersections *inters);

t_tuple *create_vector(ld *coord);
t_tuple *create_point(ld *coord);
t_tuple     *tuple_constractor(ld x, ld y,
                ld z, ld w);
bool		cmp_tuple(t_tuple *t1, t_tuple *t2);
t_tuple		*add_t(t_tuple *t1, t_tuple *t2);
t_tuple		*sub_t(t_tuple *t1, t_tuple *t2);
t_tuple		*neg_t(t_tuple *t);
t_tuple		*scalar_mult(t_tuple *t , ld scalar);
ld	magn(t_tuple *t);
t_tuple		*vec_norm(t_tuple *t);
ld	dot_product(t_tuple *t1, t_tuple *t2);
t_tuple		*cross_product(t_tuple *t1, t_tuple *t2);
bool		ldbl_cmp(ld a, ld b);
/*****************************************************/
t_matrix *matrix_constractor(int row, int col, ld *arr, size_t arr_s);
t_matrix    *identity_matrix(int row, int col);
bool        cmp_matrix(t_matrix *m1, t_matrix *m2);
t_matrix    *matrix_multi(t_matrix *m1, t_matrix *m2);
t_matrix    *matrix_tuple(t_matrix *m, t_tuple *t);
t_matrix    *matrix_transpos(t_matrix *m);
t_matrix *matrix_inverte(t_matrix *m);
ld matrix_determinant(t_matrix *m);
ld matrix_cofactor(t_matrix *m, int row, int col);
t_matrix *submatrix(t_matrix *m, int r, int c);
ld det_small_matrix(t_matrix *m);
t_tuple *matrix_to_tuple(t_matrix *m);
/*****************************************************/
t_matrix *translation(ld x, ld y, ld z);
t_matrix *scaling(ld x, ld y, ld z);
t_matrix *rotation_x(ld angle);
t_matrix *rotation_y(ld angle);
t_matrix *rotation_z(ld angle);
t_matrix *shearing(t_shear *param);
/*****************************************************/
// t_sphere *sphere();
void sphere(t_sphere **sphere, t_matrix *tran, bool flag);
t_ray *ray_constrctor(t_tuple *origin, t_tuple *vect);
t_tuple *position(t_ray *ray, ld t);
t_inters *intersection(ld p, t_sphere *obj);
t_intersections *intersections(int count, t_inters **list_objs);
t_ray *transform(t_ray *ray, t_matrix *m);
t_intersections *intersect(t_ray *ray, t_sphere *s);
t_inters *hit(t_intersections *inters);
void set_transform(t_sphere **s, t_matrix *transformation);
/*****************************************************/
t_material *material();
t_light *point_light(t_color *color, t_tuple *position);
t_tuple *normal_at(t_sphere *s, t_tuple *point);
t_tuple *reflect(t_tuple *in, t_tuple *normal);
t_color *hadamard(t_color *c1, t_color *c2);
t_color *color_scalar(t_color *c, ld s);
t_color *color_add(t_color *c1, t_color *c2);
// int32_t combine_color(t_color c);
t_color *split_color(int c);
t_color *create_color(ld *vals);
t_color *lighting(t_material *material, t_light *light, 
    t_tuple *position, t_tuple *eyev, t_tuple *normalv);
/*****************************************************/
// t_matrix *view_transformation(t_tuple *from, t_tuple *to, t_tuple *up);
/*****************************************************/
t_world *world();
t_intersections *intersect_concatenate(t_intersections *arr1, t_intersections *arr2);
// void sort_intersctions(t_intersections **inters);
t_intersections *intersect_world(t_world *world, t_ray *ray);
// t_comp *prepare_computations(t_inters *intersection, t_ray *ray);
// t_color *shade_hit(t_world *world, t_comp *comp);
// t_color *color_at(t_world *w, t_ray *ray);
/*****************************************************/
t_camera *camera(ld hsize, ld vsize, ld field_ov, t_matrix *m);
t_ray *ray_for_camera(t_camera *cam, ld x, ld y);

t_canva *create_canvas(mlx_t *p, unsigned long width, unsigned long height);











t_tuple s_create_tuple(ld *coord, int type);
t_stack_ray s_ray_constrctor(t_tuple *origin, t_tuple *vect);
t_stack_ray s_transform(t_stack_ray *ray, t_matrix *m);
t_tuple s_matrix_to_tuple(t_matrix *m);
t_tuple s_matrix_to_tuple1(t_matrix *m);
t_tuple s_position(t_stack_ray *ray, ld t);
t_matrix s_matrix_tuple(t_matrix *m, t_tuple *t);
t_inters s_hit(t_stack_intersections inters);
void s_intersect(t_stack_intersections *xs, t_stack_ray ray, t_sphere *s);
t_matrix s_matrix_constractor(int row, int col, ld *arr);
// t_tuple s_sub_t(t_tuple t1, t_tuple t2);
// t_tuple s_vec_norm(t_tuple t);
t_matrix  s_matrix_multi(t_matrix *m1, t_matrix *m2);
void s_add_t(t_tuple *res, t_tuple t1, t_tuple t2);
void s_scalar_mult(t_tuple *res, t_tuple t, ld scalar);
void s_normal_at(t_tuple *normal, t_sphere *s, t_tuple *point);
void s_vec_norm(t_tuple *t);
t_matrix s_matrix_transpos(t_matrix *m);
void s_scalar_mult(t_tuple *res, t_tuple t, ld scalar);
void s_sub_t(t_tuple *res, t_tuple t1, t_tuple t2);
void s_add_t(t_tuple *res, t_tuple t1, t_tuple t2);
t_tuple s_neg_t(t_tuple t);
void s_neg_t1(t_tuple *t);
t_color s_lighting(t_material material, t_light light, t_tuple position,
    t_tuple eyev, t_tuple normalv, bool in_shadow);
// t_color s_lighting(t_material material, t_light light, 
//     t_tuple position, t_tuple eyev, t_tuple normalv);
void copy_matrix_contant(t_matrix *m);
t_comp prepare_computations(t_inters *intersection, t_stack_ray *ray);
t_color shade_hit(t_world *world, t_comp *comp);
t_color color_at(t_world *w, t_stack_ray *ray);
t_matrix view_transformation(t_tuple *from, t_tuple *to, t_tuple *up);
t_stack_ray s_ray_for_camera(t_camera *cam, ld x, ld y);
void copy_matrix_contant(t_matrix *m);
void s_intersect_world(t_stack_intersections *in, t_world world, t_stack_ray ray);
void s_intersect_concatenate(t_stack_intersections *arr1, t_stack_intersections *arr2);
// bool is_shadowed(t_world *world, t_tuple point);
bool is_shadowed(t_world *world, t_tuple point, t_light *light);

void cone(t_cone **cone, t_matrix *trans, bool flag);
void cone_intersct(t_stack_intersections *xs, t_cone *cone, t_stack_ray *ray);
void s_cone_normal(t_tuple *norm, t_cone *cone, t_tuple *point);
void cylinder(t_cylinder **cylinder, t_matrix *trans, bool flag);
bool check_cap_cylinder(t_stack_ray *ray, ld t);
void intersect_caps_cylinder(t_cylinder *cylinder, t_stack_ray *ray, t_stack_intersections *xs);
void cylinder_intersct(t_stack_intersections *xs, t_cylinder *cylinder, t_stack_ray *ray);
void s_cylinder_normal(t_tuple *norm, t_cylinder *cylinder, t_tuple *point);

void plane(t_plane **plane, t_tuple *origin, t_tuple *vect);
void plane_intersct(t_stack_intersections *xs, t_plane *plane, t_stack_ray *ray);
void s_plane_normal(t_tuple *norm, t_plane *plane, t_tuple *point);
t_color shade_hit_with_ambient(t_world *world, t_comp *comp);
void print_matrix(t_matrix *m, const char *name);
t_material* get_material(t_comp *comp);

t_matrix *create_cone_transform(t_tuple *position, t_tuple *axis, 
                                ld radius, ld height);
t_matrix *create_cylinder_transform(t_tuple *position, t_tuple *axis, 
                                    ld radius, ld height);
t_matrix *create_plane_transform(t_tuple *origin, t_tuple *normal);

t_world s_world(t_scene *scene);
void    print_world(t_world *world);







/*-----------------------------------------------------------*/
void parsing(int fd);
t_scene *linking_fucntion(t_scene *scene, bool set);
void    set_scene(t_scene *scene, bool flag);
void jassim_mlx(t_scene *scene);
void obj_creator_sphere(t_obj *obj, t_sphere_fb *src, t_ambient_light_fb *ambient_light);
void obj_creator_plane(t_obj *obj, t_plane_fb *src, t_ambient_light_fb *ambient_light);
void    close_window(mlx_key_data_t keydata, void* param);
int number_of_all_objects(t_scene *scene);
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
// fahd
// Texture functions
void sphere_uv(t_tuple *point, t_sphere *sphere, ld *u, ld *v);
void plane_uv(t_tuple *point, t_plane *plane, ld *u, ld *v);
void cylinder_uv(t_tuple *point, t_cylinder *cylinder, ld *u, ld *v);
void cone_uv(t_tuple *point, t_cone *cone, ld *u, ld *v);

// Your existing texture functions (from parsing.h)
double sample_height(t_texture *tex, double u, double v);
t_vec3 sample_color(t_texture *tex, double u, double v);
t_tuple *cross_product(t_tuple *t1, t_tuple *t2);
t_tuple s_cross_product(t_tuple *t1, t_tuple *t2);
void plane_uv_with_dimensions(t_tuple *point, t_plane *plane, ld *u, ld *v);

void plane_uv_autodetect(t_tuple *point, t_plane *plane, ld *u, ld *v);
void plane_uv_simple_fix(t_tuple *point, t_plane *plane, ld *u, ld *v);
void s_plane_normal_bonus(t_tuple *local_normal,
	t_plane *plane, t_tuple *point);
void s_cylinder_normal_bonus(t_tuple *local_normal,
	t_cylinder *cylinder, t_tuple *point);
#endif