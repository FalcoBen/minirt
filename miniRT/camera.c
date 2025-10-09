#include "camera.h"
#include <math.h>

static void setup_camera_coordinate_system(t_virtual_camera *cam, t_vec3 forward)
{
    t_vec3 world_up;
    t_vec3 forward_norm;

    world_up = (t_vec3){0, 1, 0};
    forward_norm = vec_normalize(forward);

    // Check if forward is nearly parallel to world_up
    if (fabs(vec_dot(forward_norm, world_up)) > 0.999)
    {
        world_up = (t_vec3){0, 0, 1}; // Use z-axis as alternative up
    }

    cam->right = vec_normalize(vec_cross(forward_norm, world_up));
    cam->up = vec_normalize(vec_cross(cam->right, forward_norm));
    cam->forward = forward_norm;
}

t_virtual_camera *init_camera(t_scene *scene)
{
    t_virtual_camera *cam;
    double aspect_ratio;
    double fov_radians;
    t_vec3 world_up;

    cam = malloc(sizeof(t_virtual_camera));
    if (!cam)
        return (NULL);

    cam->origin = (t_vec3){scene->camera->coor_camera->x, scene->camera->coor_camera->y, scene->camera->coor_camera->z};
    
    t_vec3 forward = (t_vec3){scene->camera->vector_camera->x, scene->camera->vector_camera->y, scene->camera->vector_camera->z};
    
    forward = vec_normalize(forward); // camera direction
    
    world_up = (t_vec3){0, 1, 0};
    //  calculate camera coordinate system
    cam->right = vec_normalize(vec_cross(forward, world_up));
    cam->up = vec_normalize(vec_cross(cam->right, forward));
    cam->forward = forward;

    //  calculate viewport dimensions
    aspect_ratio = (double)WIDTH / (double)HEIGHT;
    fov_radians = scene->camera->angle_view * (M_PI / 180.0);
    cam->viewport_height = 2.0 * tan(fov_radians / 2.0);
    cam->viewport_width = aspect_ratio * cam->viewport_height;

    // calculate viewport vectors
    cam->horizontal = vec_scale(cam->right, cam->viewport_width);
    cam->vertical = vec_scale(cam->up, cam->viewport_height);

    // calculate lower left corner
    // viewport is 1 unit in front of camera along forward direction
    t_vec3 viewport_center = vec_add(cam->origin, cam->forward);
    cam->lower_left = vec_sub(viewport_center, vec_scale(cam->horizontal, 0.5));
    cam->lower_left = vec_sub(cam->lower_left, vec_scale(cam->vertical, 0.5));

    return (cam);
}

t_ray get_ray(t_virtual_camera *cam, double u, double v)
{
    t_ray ray;
    t_vec3 direction;
    t_vec3 point_on_viewport;

    // Calculate the point on the viewport: lower_left + u*horizontal + v*vertical
    point_on_viewport = cam->lower_left;
    point_on_viewport = vec_add(point_on_viewport, vec_scale(cam->horizontal, u));
    point_on_viewport = vec_add(point_on_viewport, vec_scale(cam->vertical, v));
    
    // The ray direction is from camera origin to this point on viewport
    direction = vec_sub(point_on_viewport, cam->origin);
    
    ray.origin = cam->origin;
    ray.direction = vec_normalize(direction);
    
    return (ray);
}