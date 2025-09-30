#ifndef RAYS_H
#define RAYS_H

#include "../MLX42/include/MLX42/MLX42.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 800

typedef struct s_vec3
{
    double x;
    double y;
    double z;
} t_vec3;

typedef struct s_color
{
    int r;
    int g;
    int b;
} t_color;

typedef struct s_ray
{
    t_vec3 orig;
    t_vec3 dir;
} t_ray;

typedef struct s_sphere
{
    t_vec3 center;
    double radius;
} t_sphere;

typedef struct s_hit_record
{
    t_vec3 p;
    double t;
} t_hit_record;

t_vec3 vec_add(t_vec3 a, t_vec3 b);
t_vec3 vec_sub(t_vec3 a, t_vec3 b);
t_vec3 vec_scale(t_vec3 v, double scalar);
double vec_dot(t_vec3 a, t_vec3 b);
t_vec3 vec_cross(t_vec3 a, t_vec3 b);
double vec_length(t_vec3 v);
t_vec3 vec_normalize(t_vec3 v);
t_vec3 vec_mult(t_vec3 a, t_vec3 b);

t_vec3 ray_at(t_ray *r, double t);
t_color ray_color(t_ray *r, t_sphere *sphere);

int hit_sphere(t_sphere *sphere, t_ray *r, double t_min, double t_max, t_hit_record *rec);

void render_mlx_gradient(mlx_t *mlx, mlx_image_t *img);

#endif