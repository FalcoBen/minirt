#include "minirt.h"
#include "camera.h"
#include <math.h>

static uint32_t color_to_int(t_color color)
{
    return ((color.r << 24) | (color.g << 16) | (color.b << 8) | 0xFF);
}

// double intersect_plane(t_ray *ray, t_plane *plane, t_hit_record *rec)
// {
//     t_vec3 plane_point = *plane->coor_plane;
//     t_vec3 plane_normal = vec_normalize(*plane->vector_plane);
//     t_vec3 oc = vec_sub(plane_point, ray->origin);
//     double denom = vec_dot(ray->direction, plane_normal);

//     // printf("Denom: %f\n", denom);
//     if (fabs(denom) < 1e-6)
//         return INFINITY;
    
//     double t = vec_dot(oc, plane_normal) / denom;
//     if (t < 0)
//     {
//         // printf("---------returned\n");
//         return INFINITY;
//     }

//     if (rec)
//     {
//         rec->t = t;
//         rec->p = vec_add(ray->origin, vec_scale(ray->direction, t));
//         rec->normal = plane_normal;
//         rec->color = *plane->color_plane;
//     }
    
//     return t;
// }

double intersect_plane(t_ray *ray, t_plane *plane, t_hit_record *rec)
{
    t_vec3 plane_point = *plane->coor_plane;
    t_vec3 plane_normal = vec_normalize(*plane->vector_plane);
    t_vec3 oc = vec_sub(plane_point, ray->origin);
    double denom = vec_dot(ray->direction, plane_normal);

    if (fabs(denom) < 1e-6)
        return INFINITY;
    
    double t = vec_dot(oc, plane_normal) / denom;
    if (t < 0)
        return INFINITY;

    if (rec)
    {
        rec->t = t;
        rec->p = vec_add(ray->origin, vec_scale(ray->direction, t));
        rec->normal = plane_normal;
        rec->color = *plane->color_plane;

        // Compute UV coordinates for plane
        double scale = 10.0; // Tiling scale (adjust for texture frequency)
        t_vec3 to_hit = vec_sub(rec->p, plane_point); // Vector from plane origin to hit point
        t_vec3 arbitrary = (fabs(plane_normal.x) > 0.9) ? (t_vec3){0,1,0} : (t_vec3){1,0,0}; // Avoid parallel to normal
        t_vec3 tangent_u = vec_normalize(vec_cross(plane_normal, arbitrary));
        t_vec3 tangent_v = vec_cross(plane_normal, tangent_u);

        rec->u = fmod(vec_dot(to_hit, tangent_u) / scale, 1.0);
        if (rec->u < 0)
        {
            rec->u += 1.0; // Ensure [0,1]
        } 
        rec->v = fmod(vec_dot(to_hit, tangent_v) / scale, 1.0);
        if (rec->v < 0)
        {
            rec->v += 1.0; // Ensure [0,1]
        } 
        rec->hit_object = plane; // Store plane as hit object
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
        // printf("rec->normal.y = ''%f''\n", -rec->normal.y);
        double theta = acos(-rec->normal.y);
        // printf("theta = [[%f]]\n", theta);
        double phi = atan2(-rec->normal.z, rec->normal.x) + M_PI; 
        rec->u = phi / (2 * M_PI); 
        rec->v = theta / M_PI;     
        rec->hit_object = sphere;
        // printf("rec->u = [%f]\n", rec->u); 
        // printf("rec->v =]%f[\n", rec->v); 
        
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
#include <stdint.h>

uint32_t mlx_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((uint32_t)a << 24)
         | ((uint32_t)r << 16)
         | ((uint32_t)g << 8)
         | ((uint32_t)b);
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
            print_ray_debug(&ray, x, y);

            double closest_t = INFINITY;
            t_hit_record hit_rec;
            int hit = 0;

            t_plane *curr_plane = scene->plane;
            while (curr_plane)
            {
                double t = intersect_plane(&ray, curr_plane, NULL);
                if (t < closest_t)
                {
                    closest_t = t;
                    intersect_plane(&ray, curr_plane, &hit_rec);
                    // printf("Plane hit at pixel [%d, %d], t = %f\n", x, y, t);
                    hit = 1;
                }
                curr_plane = curr_plane->next;
            }

            t_sphere *curr_sphere = scene->sphere;
            while (curr_sphere)
            {
                double t = intersect_sphere(&ray, curr_sphere, NULL);
                if (t < closest_t)
                {
                    closest_t = t;
                    intersect_sphere(&ray, curr_sphere, &hit_rec);
                    // printf("Sphere hit at pixel [%d, %d], t = %f\n", x, y, t);
                    hit = 1;
                }
                curr_sphere = curr_sphere->next;
            }
            
            t_color final_color = {0,0,0};
            if (hit)
            {
                t_vec3 hit_point = hit_rec.p;
                t_vec3 normal = hit_rec.normal;
                t_vec3 obj_color = {hit_rec.color.r / 255.0, hit_rec.color.g / 255.0, hit_rec.color.b / 255.0};
                
                /*--------------------------------------------------------------------------*/
                t_texture *bump_tex = NULL;
                if (hit_rec.hit_object)
                {
                    t_sphere *hit_sphere = (t_sphere *)hit_rec.hit_object;
                    if (hit_sphere && hit_sphere->flag_bump && hit_sphere->bump_texture) 
                    {
                        obj_color = sample_color(hit_sphere->bump_texture, hit_rec.u, hit_rec.v);
                    }
                    bump_tex = hit_sphere->bump_texture;
                    if (bump_tex)
                    {   
                        // printf("Sphere hit at pixel [%d, %d], u=%f, v=%f\n", x, y, hit_rec.u, hit_rec.v);
                        double offset = 0.001;
                        double h = sample_height(bump_tex, hit_rec.u, hit_rec.v);
                        double hu = sample_height(bump_tex, hit_rec.u + offset, hit_rec.v);
                        double hv = sample_height(bump_tex, hit_rec.u, hit_rec.v + offset);
                        
                        double dHdu = (hu - h) / offset;
                        double dHdv = (hv - h) / offset;
                        
                        // Compute tangent space
                        t_vec3 arbitrary = (fabs(normal.x) > 0.9) ? (t_vec3){0,1,0} : (t_vec3){1,0,0};
                        t_vec3 tangent_u = vec_normalize(vec_cross(normal, arbitrary));
                        t_vec3 tangent_v = vec_cross(normal, tangent_u);
                        
                        // Perturb normal
                        normal = vec_normalize(vec_add(normal, vec_add(vec_scale(tangent_u, dHdu * bump_tex->scale),
                                                                    vec_scale(tangent_v, dHdv * bump_tex->scale))));
                                                                }
                }
                /*--------------------------------------------------------------------------*/
                
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
                
                final_color.r = (int)(color.x * 255);
                final_color.g = (int)(color.y * 255);
                final_color.b = (int)(color.z * 255);
            }
            // Call debug function
            // debug_rendering(scene, cam, img, x, y, ray, closest_t, hit_rec, final_color, hit);
            // if(bumped)
            // {
            //     mlx_put_pixel(img, x, y, mlx_rgba(255, 0, 0, 128));
            //     // bumped = false;
            // }
            // else
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
    // mlx_terminate(mlx);
}