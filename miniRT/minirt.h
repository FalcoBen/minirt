#ifndef MINI_RT_H
#define MINI_RT_H

#include "../MLX42/include/MLX42/MLX42.h"
#include "../get_next_line/get_next_line.h"
// #include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "colors.h"
// # include "camera.h"

#define WIDTH 2000
#define HEIGHT 1400



typedef struct s_tuple t_tuple;
typedef struct s_scene t_scene;
typedef struct s_objects
{
    char *identifier;
    int nb;
    void (*assign_object)(char **data, t_scene *scene);
    struct s_objects *next;
} t_objects;

typedef struct s_container
{
    char *line;
    struct s_container *next;
} t_container;

typedef struct s_color
{
    int r;
    int g;
    int b;
} t_color;

typedef struct s_ambient_light
{
    double bright_ambient_light;
    t_color *color_ambient_light;
    struct s_ambient_light *next;
} t_ambient_light;

typedef struct s_vec3
{
    double x;
    double y;
    double z;
    double w;
} t_vec3;

typedef struct s_camera
{
    t_vec3 *coor_camera;
    t_vec3 *vector_camera;
    double angle_view;
    struct s_camera *next;
} t_camera;

typedef struct s_light
{
    t_vec3 *coor_light;
    double bright_light;
    t_color *color_light;
    struct s_light *next;
} t_light;

typedef struct s_plane
{
    t_vec3 *coor_plane;
    t_color *color_plane;
    t_vec3 *vector_plane;
    struct s_plane *next;
} t_plane;

typedef struct s_sphere
{
    t_vec3 *coor_sphere;
    double diameter_sphere;
    t_color *color_sphere;
    struct s_sphere *next;
} t_sphere;

typedef struct s_cylinder
{
    t_vec3 *coor_cylinder;
    t_vec3 *vector_cylinder;
    double diameter_cylinder;
    double height_cylinder;
    t_color *color_cylinder;
    struct s_cylinder *next;
} t_cylinder;

typedef struct s_scene
{
    t_objects *object;
    t_ambient_light *ambient_light; //single instance
    t_camera *camera; //single instance
    t_light *light; // llinked list
    t_plane *plane; // llinked list
    t_sphere *sphere; // llinked list
    t_cylinder *cylinder;  // llinked list
    // struct s_scene *next;
} t_scene;

typedef struct s_all_data
{
    t_container *container;

} t_all_data;

#define ld double
/*====================jbahmida===========================*/

typedef struct s_color_ja
{
    ld r;
    ld g;
    ld b;
} t_color_ja;

typedef struct s_material
{
    t_color_ja *color_ja;
    ld ambient;
    ld diffuse;
    ld specular;
    ld shininess;
} t_material;

typedef struct s_sphere_ja
{
    
    int id;
    t_tuple *origin;
    ld radius;
    // t_matrix transform;
    // t_matrix inv;
    t_material *material;
} t_sphere_ja;

typedef union s_obj
{
	t_sphere_ja *sphere_ja;
	// t_cylinder_ja *cylinder_ja;
	// t_plane_ja *plan_ja;
}t_obj;


typedef enum  type
{
    PLANE,
    SPHERE,
    CYLINDER,
} t_type;

typedef struct s_object_ja
{
	t_type type;
	t_obj *obj;
}t_object_ja;


typedef struct s_world
{
	int obj_num;
    t_light *light;
	t_object_ja **objects;
}t_world;

typedef struct s_tuple
{
    ld x;
    ld y;
    ld z;
    ld w;
} t_tuple;



/*-----------linked list----------------------*/
void ft_lstadd_back(t_container **lst, t_container *new);
void ft_lstadd_front(t_container **lst, t_container *new);
t_container *ft_lstnew(void *content);
t_container *ft_lstlast(t_container *lst);
void ft_lstclear(void **lst, void (*del)(void *), char c);
void ft_lstdelone(void *lst, void (*del)(void *), char c);
/*------------------------------------------------------*/
char **ft_split(char const *s, char c);
char **ft_split_white(char *str);
/*-------------objects--------------*/
void init_objects(t_objects **input_data, char ***tokens,int counter);
double ft_atoi_double(char *str);
int ft_atoi_color(char *str, char *scene);
bool non_num_chara(char *str, int i);
void skip_spaces(char *str, int *i);
void del(void *content);
void	exit_error(char *str, char *scene, void *data, bool flag);
void	ft_free_split(char **str);



void ft_ambient_light(char **data, t_scene *scene);
void ft_cylinder(char **data, t_scene *scene);
void ft_camera(char **data, t_scene *scene);
void ft_sphere(char **data, t_scene *scene);
void ft_plane(char **data, t_scene *scene);
void ft_light(char **data, t_scene *scene);
/*********initiaie_scenes***************/
void initialize_ambient_light(t_scene *scene);
void initialize_light(t_scene *scene);
void initialize_plane(t_scene *scene);
void initialize_spher(t_scene *scene);
void initialize_cylinder(t_scene *scene);
void initialize_camera(t_scene *scene);

/************************/
void printer(t_scene *scene);
void process_scene_tokens(char ***tokens, int counter, t_scene *scene);
void init_object_dispatch_table(t_objects **dispatch_table);
void add_object(t_objects **head, char *identifier, void (*assign_object)(char **, t_scene *));


/*----------------------------mlx--------------------------*/
void start_using_mlx(t_scene *scene);
/*--------------------------math vec3----------------------*/

t_vec3 vec_add(t_vec3 a, t_vec3 b);
t_vec3 vec_sub(t_vec3 a, t_vec3 b);
t_vec3 vec_scale(t_vec3 v, double scalar);
t_vec3 vec_cross(t_vec3 a, t_vec3 b);
t_vec3 vec_normalize(t_vec3 v);
double vec_dot(t_vec3 a, t_vec3 b);
double vec_length(t_vec3 v);
t_vec3 vec_mult(t_vec3 a, t_vec3 b);  

/*-------------------------------------------------------*/

t_world s_world(t_scene *scene);
void    print_world(t_world *world);

#endif