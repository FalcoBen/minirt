#include "minirt.h"
#include "camera.h"
#include <math.h>
void debug_rendering(t_scene *scene, t_virtual_camera *cam, mlx_image_t *img, int x, int y, t_ray ray, double closest_t, t_hit_record hit_rec, t_color final_color, int hit)
{
    printf("=== Debug at Pixel (%d, %d) ===\n", x, y);

    // Camera and Ray Debug
    printf("Camera Origin: (%f, %f, %f)\n", cam->origin.x, cam->origin.y, cam->origin.z);
    printf("Forward: (%f, %f, %f)\n", cam->forward.x, cam->forward.y, cam->forward.z);
    printf("Right: (%f, %f, %f)\n", cam->right.x, cam->right.y, cam->right.z);
    printf("Up: (%f, %f, %f)\n", cam->up.x, cam->up.y, cam->up.z);
    printf("Viewport Width: %f, Height: %f\n", cam->viewport_width, cam->viewport_height);
    printf("Ray Origin: (%f, %f, %f), Direction: (%f, %f, %f)\n", ray.origin.x, ray.origin.y, ray.origin.z, ray.direction.x, ray.direction.y, ray.direction.z);

    // Intersection Debug
    printf("Closest t: %f, Hit: %d\n", closest_t, hit);
    if (hit)
    {
        printf("Hit Point: (%f, %f, %f)\n", hit_rec.p.x, hit_rec.p.y, hit_rec.p.z);
        printf("Normal: (%f, %f, %f)\n", hit_rec.normal.x, hit_rec.normal.y, hit_rec.normal.z);
        printf("Color from Hit: (%d, %d, %d)\n", hit_rec.color.r, hit_rec.color.g, hit_rec.color.b);
    }

    // Scene Object Debug
    printf("Sphere at: (%f, %f, %f), Radius: %f\n", scene->sphere->coor_sphere->x, scene->sphere->coor_sphere->y, scene->sphere->coor_sphere->z, scene->sphere->diameter_sphere / 2.0);
    t_plane *p = scene->plane;
    int plane_count = 0;
    while (p)
    {
        printf("Plane %d at: (%f, %f, %f), Normal: (%f, %f, %f)\n", plane_count, p->coor_plane->x, p->coor_plane->y, p->coor_plane->z, p->vector_plane->x, p->vector_plane->y, p->vector_plane->z);
        p = p->next;
        plane_count++;
    }

    // Final Color Debug
    printf("Final Color: (%d, %d, %d)\n", final_color.r, final_color.g, final_color.b);
    printf("---------------------------\n");
}
static uint32_t color_to_int(t_color color)
{
    return ((color.r << 24) | (color.g << 16) | (color.b << 8) | 0xFF);
}

double intersect_plane(t_ray *ray, t_plane *plane, t_hit_record *rec)
{
    t_vec3 plane_point = *plane->coor_plane;
    t_vec3 plane_normal = vec_normalize(*plane->vector_plane);
    t_vec3 oc = vec_sub(plane_point, ray->origin);
    double denom = vec_dot(ray->direction, plane_normal);

    // printf("Denom: %f\n", denom);
    if (fabs(denom) < 1e-6)
        return INFINITY;
    
    double t = vec_dot(oc, plane_normal) / denom;
    if (t < 0)
    {
        // printf("---------returned\n");
        return INFINITY;
    }

    if (rec)
    {
        rec->t = t;
        rec->p = vec_add(ray->origin, vec_scale(ray->direction, t));
        rec->normal = plane_normal;
        rec->color = *plane->color_plane;
    }
    
    return t;
}

double intersect_sphere(t_ray *ray, t_sphere *sphere, t_hit_record *rec)
{
    t_vec3 oc = vec_sub(ray->origin, *sphere->coor_sphere);
    double a = vec_dot(ray->direction, ray->direction);
    double b = 2.0 * vec_dot(oc, ray->direction);
    double c = vec_dot(oc, oc) - (sphere->diameter_sphere / 2.0) * (sphere->diameter_sphere / 2.0);
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0.0)
        return INFINITY;
    
    double t = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t < 0)
    {
        t = (-b + sqrt(discriminant)) / (2.0 * a);
        if (t < 0.0)
            return INFINITY;
    }
    
    if (rec)
    {
        // puts("=============================================================\n");
        rec->t = t;
        rec->p = vec_add(ray->origin, vec_scale(ray->direction, t));
        rec->normal = vec_normalize(vec_sub(rec->p, *sphere->coor_sphere)); // Normal = (P - center) / radius
        rec->color = *sphere->color_sphere;
    }

    return t;
}

char *get_color(t_color *curr_color)
{
    char *color = NULL;
    if(curr_color->r == 70 && curr_color->g == 130 && curr_color->b == 180)
    {
        color = ft_strdup("Blue");
    }
    else if(curr_color->r == 255 && curr_color->g == 0 && curr_color->b == 0)
        color = ft_strdup("-------------------------------Red--------------------------");
    else
        color = ft_strdup("--Nothing");
    return color;
}

void    close_window(mlx_key_data_t keydata, void* param)
{
    if(keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        printf("window closed !!!!!\n");
        exit(0);
    }
}

void start_using_mlx(t_scene *scene)
{
    mlx_t *mlx;
    mlx_image_t *img;
    int x, y;
    t_virtual_camera *cam;
    t_ray ray;

    mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
    // if (!mlx)
    //     exit_error("MLX initialization failed", NULL);
    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    // if (!img)
    //     exit_error("MLX image creation failed", NULL);

    cam = init_camera(scene);
    // if (!cam)
    //     exit_error("Camera initialization failed", NULL);
    print_camera_debug(cam);
    if (scene->plane)
        print_plane_debug(scene->plane);

    // Rendering loop
    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            double u = (double)x / (WIDTH - 1);
            double v = 1.0 - (double)y / (HEIGHT - 1); // Flip Y (0 at top)

            ray = get_ray(cam, u, v);
            print_ray_debug(&ray, x, y); // Existing debug

            double closest_t = INFINITY;
            t_hit_record hit_rec;
            int hit = 0; // Flag for any hit

            // Check all planes
            t_plane *curr_plane = scene->plane;
            while (curr_plane)
            {
                double t = intersect_plane(&ray, curr_plane, NULL);
                if (t < closest_t)
                {
                    closest_t = t;
                    intersect_plane(&ray, curr_plane, &hit_rec);
                    printf("Plane hit at pixel [%d, %d], t = %f\n", x, y, t);
                    hit = 1;
                }
                curr_plane = curr_plane->next;
            }

            // Check all spheres
            t_sphere *curr_sphere = scene->sphere;
            while (curr_sphere)
            {
                double t = intersect_sphere(&ray, curr_sphere, NULL);
                if (t < closest_t)
                {
                    closest_t = t;
                    intersect_sphere(&ray, curr_sphere, &hit_rec);
                    printf("Sphere hit at pixel [%d, %d], t = %f\n", x, y, t);
                    hit = 1;
                }
                curr_sphere = curr_sphere->next;
            }

            // If hit, compute color with lighting
            t_color final_color = {0,0,0}; // Black by default
            if (hit)
            {
                t_vec3 hit_point = hit_rec.p;
                t_vec3 normal = hit_rec.normal;
                t_vec3 obj_color = {hit_rec.color.r / 255.0, hit_rec.color.g / 255.0, hit_rec.color.b / 255.0};

                // Ambient lighting
                t_ambient_light *amb = scene->ambient_light;
                t_vec3 amb_color = {amb->color_ambient_light->r / 255.0, 
                                   amb->color_ambient_light->g / 255.0, 
                                   amb->color_ambient_light->b / 255.0};
                t_vec3 color = vec_mult(obj_color, vec_scale(amb_color, amb->bright_ambient_light));

                // Diffuse lighting
                t_light *current_light = scene->light;
                while (current_light)
                {
                    t_vec3 light_pos = {current_light->coor_light->x, 
                                       current_light->coor_light->y, 
                                       current_light->coor_light->z};
                    t_vec3 to_light = vec_sub(light_pos, hit_point);
                    double dist = vec_length(to_light);
                    t_vec3 light_dir = vec_normalize(to_light);

                    double diff_factor = fmax(vec_dot(normal, light_dir), 0.0);
                    double atten = 1.0;

                    t_vec3 light_color = {current_light->color_light->r / 255.0, 
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

                // Convert to t_color
                final_color.r = (int)(color.x * 255);
                final_color.g = (int)(color.y * 255);
                final_color.b = (int)(color.z * 255);
            }

            // Call debug function
            // debug_rendering(scene, cam, img, x, y, ray, closest_t, hit_rec, final_color, hit);

            mlx_put_pixel(img, x, y, color_to_int(final_color));
            x++;
        }
        y++;
    }

    if (mlx_image_to_window(mlx, img, 0, 0) < 0)
        perror("mlx_image to window:");

    mlx_key_hook(mlx, &close_window, mlx);
    mlx_loop(mlx);

    free(cam);
    mlx_delete_image(mlx, img);
    mlx_terminate(mlx);
}