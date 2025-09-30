#include "rays.h"

t_vec3 vec_add(t_vec3 a, t_vec3 b)
{
    return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vec3 vec_sub(t_vec3 a, t_vec3 b)
{
    return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}

t_vec3 vec_scale(t_vec3 v, double scalar)
{
    return ((t_vec3){v.x * scalar, v.y * scalar, v.z * scalar});
}

double vec_dot(t_vec3 a, t_vec3 b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3 vec_cross(t_vec3 a, t_vec3 b)
{
    return ((t_vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    });
}

double vec_length(t_vec3 v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vec3 vec_normalize(t_vec3 v)
{
    double length = vec_length(v);
    if (length == 0.0)
        return ((t_vec3){0, 0, 0});
    return ((t_vec3){v.x / length, v.y / length, v.z / length});
}

t_vec3 vec_mult(t_vec3 a, t_vec3 b)
{
    return ((t_vec3){a.x * b.x, a.y * b.y, a.z * b.z});
}

t_vec3 ray_at(t_ray *r, double t)
{
    return vec_add(r->orig, vec_scale(r->dir, t));
}

int hit_sphere(t_sphere *sphere, t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
    t_vec3 oc = vec_sub(r->orig, sphere->center);
    double a = vec_dot(r->dir, r->dir);
    double b = 2.0 * vec_dot(oc, r->dir);
    double c = vec_dot(oc, oc) - sphere->radius * sphere->radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return 0;

    double t = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t < t_min || t > t_max)
    {
        t = (-b + sqrt(discriminant)) / (2.0 * a);
        if (t < t_min || t > t_max)
            return 0;
    }

    rec->t = t;
    rec->p = ray_at(r, t);
    return 1;
}

t_color ray_color(t_ray *r, t_sphere *sphere)
{
    t_hit_record rec;
    if (hit_sphere(sphere, r, 0.0, 1e30, &rec))
        return (t_color){255, 0, 0}; // Red for sphere hit

    t_vec3 unit_direction = vec_normalize(r->dir);
    double a = 0.5 * (unit_direction.y + 1.0);
    t_vec3 white = {1.0, 1.0, 1.0};
    t_vec3 blue = {0.5, 0.7, 1.0};
    t_vec3 color = vec_add(vec_scale(white, 1.0 - a), vec_scale(blue, a));
    return (t_color){(int)(color.x * 255.999), (int)(color.y * 255.999), (int)(color.z * 255.999)};
}

static uint32_t color_to_int(t_color color)
{
    return ((color.r << 24) | (color.g << 16) | (color.b << 8) | 0xFF);
}

void render_mlx_gradient(mlx_t *mlx, mlx_image_t *img)
{
    int x, y;
    double aspect_ratio = (double)WIDTH / (double)HEIGHT;
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;
    t_vec3 camera_center = {0, 0, 0};
    t_vec3 viewport_u = {viewport_width, 0, 0};
    t_vec3 viewport_v = {0, -viewport_height, 0};
    t_vec3 pixel_delta_u = vec_scale(viewport_u, 1.0 / WIDTH);
    t_vec3 pixel_delta_v = vec_scale(viewport_v, 1.0 / HEIGHT);
    t_vec3 viewport_upper_left = vec_sub(vec_sub(vec_sub(camera_center, (t_vec3){0, 0, focal_length}), vec_scale(viewport_u, 0.5)), vec_scale(viewport_v, 0.5));
    t_vec3 pixel00_loc = vec_add(viewport_upper_left, vec_scale(vec_add(pixel_delta_u, pixel_delta_v), 0.5));
    t_sphere sphere = {{0, 0, -1}, 0.5};

    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            t_vec3 pixel_center = vec_add(pixel00_loc, vec_add(vec_scale(pixel_delta_u, x), vec_scale(pixel_delta_v, y)));
            t_ray ray = {camera_center, vec_sub(pixel_center, camera_center)};
            t_color pixel_color = ray_color(&ray, &sphere);
            mlx_put_pixel(img, x, y, color_to_int(pixel_color));
            x++;
        }
        y++;
    }
}