#ifndef CAMERA_H
# define CAMERA_H

# include "minirt.h"
typedef struct s_vec3 t_vec3;
typedef struct s_scene t_scene;
// typedef t_vec3 t_vec3;
#define M_PI 3.14159265358979323846
typedef struct s_ray
{
    t_vec3	origin;
    t_vec3	direction;
}	t_ray;

typedef struct s_virtual_camera
{
    t_vec3	origin;          
    t_vec3	forward;         
    t_vec3	right;           
    t_vec3	up;              
    double	viewport_width; 
    double	viewport_height;
    t_vec3	horizontal;      
    t_vec3	vertical;        
    t_vec3	lower_left;      
}	t_virtual_camera;

/*---------------funcs-----------------*/

t_virtual_camera	*init_camera(t_scene *scene);
t_ray		get_ray(t_virtual_camera *cam, double u, double v);


void print_camera_debug(t_virtual_camera *cam);
void print_ray_debug(t_ray *ray, int x, int y);
void print_plane_debug(t_plane *plane);
#endif