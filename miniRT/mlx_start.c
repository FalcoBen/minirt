#include "minirt.h"
#include "camera.h" // Include the camera header
#include <math.h>

static uint32_t color_to_int(t_color color)
{
    return ((color.r << 24) | (color.g << 16) | (color.b << 8) | 0xFF);
}

// ray_plane intersection
double intersect_plane(t_ray *ray, t_plane *plane)
{
    t_vec3 plane_point;
    t_vec3 plane_normal;
    t_vec3 oc;
    double denom;
    double t;

    plane_point = (t_vec3){plane->coor_plane->x, plane->coor_plane->y, plane->coor_plane->z};
    plane_normal = (t_vec3){plane->vector_plane->x, plane->vector_plane->y, plane->vector_plane->z};
    plane_normal = vec_normalize(plane_normal);

    // Calculate denominator: dot(ray_direction, plane_normal)
    denom = vec_dot(ray->direction, plane_normal);
    
    // If ray is parallel to plane, no intersection
    if (fabs(denom) < 1e-6)
        return (INFINITY);
    
    // Calculate vector from plane point to ray origin: (plane_point - ray->origin)
    oc = vec_sub(plane_point, ray->origin);
    
    // Calculate t: dot(oc, plane_normal) / denom
    t = vec_dot(oc, plane_normal) / denom;
    
    // Only return positive t values (intersections in front of camera)
    if (t < 0)
        return (INFINITY);
    
    return (t);
}

void start_using_mlx(t_scene *scene)
{
    mlx_t *mlx;
    mlx_image_t *img;
    int x;
    int y;
    t_camera_obj *cam;
    t_ray ray;
 
    mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
    if (!mlx)
        exit_error("MLX initialization failed", NULL);
    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
        exit_error("MLX image creation failed", NULL);

    cam = init_camera(scene);
    if (!cam)
        exit_error("Camera initialization failed", NULL);
    print_camera_debug(cam);
    if (scene->plane)
        print_plane_debug(scene->plane);
    // rendering loop
    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            double u;
            double v;
            double closest_t;

            // Calculate normalized screen coordinates (0.0 to 1.0)
            u = (double)x / (WIDTH - 1);
            v = (double)y / (HEIGHT - 1);
            v = 1.0 - v; // Flip Y coordinate (0 is at top in screen coordinates)

            // 4. Generate ray for this pixel
            ray = get_ray(cam, u, v);
            print_ray_debug(&ray, x, y); // DEBUG
            // 5. Initialize closest intersection to "infinity" (no hit)
            closest_t = INFINITY;
            t_color final_color = {0, 0, 0}; // Start with black

            // 6. Check intersection with ALL planes in the scene
            t_plane *current_plane = scene->plane;
            t_plane *hit_plane = NULL;  // Track the hit plane
            while (current_plane)
            {
                double t = intersect_plane(&ray, current_plane);
                if (t < closest_t && t > 0.0)
                {
                    closest_t = t;
                    hit_plane = current_plane;  // Save hit plane
                }
                current_plane = current_plane->next;
            }

            // 7. If we hit a plane, compute color with lighting
            if (closest_t < INFINITY && hit_plane)
            {
                t_vec3 hit_point = vec_add(ray.origin, vec_scale(ray.direction, closest_t));
                t_vec3 normal = vec_normalize((t_vec3){hit_plane->vector_plane->x, hit_plane->vector_plane->y, hit_plane->vector_plane->z});

                // Object color as float (0-1)
                t_vec3 obj_color = (t_vec3){hit_plane->color_plane->r / 255.0,
                                            hit_plane->color_plane->g / 255.0,
                                            hit_plane->color_plane->b / 255.0};

                // Ambient contribution
                t_ambient_light *amb = scene->ambient_light;
                t_vec3 amb_color = (t_vec3){amb->color_ambient_light->r / 255.0,
                                            amb->color_ambient_light->g / 255.0,
                                            amb->color_ambient_light->b / 255.0};
                t_vec3 color = vec_mult(obj_color, vec_scale(amb_color, amb->bright_ambient_light));

                // Loop over lights for diffuse (no shadows yet)
                t_light *current_light = scene->light;
                while (current_light)
                {
                    t_vec3 light_pos = (t_vec3){current_light->coor_light->x,
                                                current_light->coor_light->y,
                                                current_light->coor_light->z};
                    t_vec3 to_light = vec_sub(light_pos, hit_point);
                    double dist = vec_length(to_light);
                    t_vec3 light_dir = vec_normalize(to_light);

                    double diff_factor = fmax(vec_dot(normal, light_dir), 0.0);

                    // Optional attenuation for distance fade (uncomment for stronger gradient)
                    // double atten = 1.0 / (dist * dist + 1e-6);
                    double atten = 1.0;  // Start without for softer effect

                    t_vec3 light_color = (t_vec3){current_light->color_light->r / 255.0,
                                                  current_light->color_light->g / 255.0,
                                                  current_light->color_light->b / 255.0};
                    double light_int = current_light->bright_light;

                    t_vec3 diffuse = vec_mult(obj_color, vec_scale(light_color, diff_factor * light_int * atten));
                    color = vec_add(color, diffuse);

                    current_light = current_light->next;
                }

                // Clamp to [0,1]
                color.x = fmin(1.0, fmax(0.0, color.x));
                color.y = fmin(1.0, fmax(0.0, color.y));
                color.z = fmin(1.0, fmax(0.0, color.z));

                // Convert back to t_color
                final_color.r = (int)(color.x * 255);
                final_color.g = (int)(color.y * 255);
                final_color.b = (int)(color.z * 255);

                mlx_put_pixel(img, x, y, color_to_int(final_color));
            }
            else
            {
                // Background color (black)
                mlx_put_pixel(img, x, y, COLOR_BLACK);
            }
            x++;
        }
        y++;
    }

    // 8. Cleanup and display
    free(cam); // Don't forget to free the camera!
    if (!mlx_image_to_window(mlx, img, 0, 0))
        perror("mlx_image_to_window");
    mlx_loop(mlx);
    mlx_delete_image(mlx, img);
    mlx_terminate(mlx);
}