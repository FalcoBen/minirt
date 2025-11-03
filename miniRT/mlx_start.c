#include "minirt.h"
#include "camera.h"
#include <math.h>



// Add these helper functions to mlx_start.c

// Check if a point is in shadow (between hit point and light)
int is_in_shadow(t_scene *scene, t_vec3 hit_point, t_vec3 light_pos)
{
    t_vec3 to_light = vec_sub(light_pos, hit_point);
    double light_distance = vec_length(to_light);
    t_vec3 light_dir = vec_normalize(to_light);
    
    // Create shadow ray slightly offset from surface to avoid self-intersection
    t_ray shadow_ray;
    shadow_ray.origin = vec_add(hit_point, vec_scale(light_dir, 0.001));
    shadow_ray.direction = light_dir;
    
    // Check if any object blocks the light
    t_plane *curr_plane = scene->plane;
    while (curr_plane)
    {
        double t = intersect_plane(&shadow_ray, curr_plane, NULL);
        if (t > 0.001 && t < light_distance)
            return 1; // In shadow
        curr_plane = curr_plane->next;
    }
    
    t_sphere *curr_sphere = scene->sphere;
    while (curr_sphere)
    {
        double t = intersect_sphere(&shadow_ray, curr_sphere, NULL);
        if (t > 0.001 && t < light_distance)
            return 1;
        curr_sphere = curr_sphere->next;
    }
    
    t_cylinder *curr_cylinder = scene->cylinder;
    while (curr_cylinder)
    {
        double t = intersect_cylinder(&shadow_ray, curr_cylinder, NULL);
        if (t > 0.001 && t < light_distance)
            return 1;
        curr_cylinder = curr_cylinder->next;
    }
    
    return 0; // Not in shadow
}

// Calculate specular reflection (Phong model)
t_vec3 calculate_specular(t_vec3 light_dir, t_vec3 normal, t_vec3 view_dir, 
                          t_vec3 light_color, double light_intensity, double shininess)
{
    // Reflect light direction around normal
    // R = 2(N·L)N - L
    double n_dot_l = vec_dot(normal, light_dir);
    t_vec3 reflect_dir = vec_sub(vec_scale(normal, 2.0 * n_dot_l), light_dir);
    reflect_dir = vec_normalize(reflect_dir);
    
    // Calculate specular factor
    double spec_factor = vec_dot(reflect_dir, view_dir);
    if (spec_factor < 0.0)
        spec_factor = 0.0;
    
    // Apply shininess (higher = smaller, sharper highlight)
    spec_factor = pow(spec_factor, shininess);
    
    // Specular contribution
    return vec_scale(light_color, spec_factor * light_intensity);
}

// Updated lighting calculation with shadows and specular
t_vec3 calculate_lighting(t_scene *scene, t_hit_record *hit_rec, t_vec3 view_dir, 
                          t_vec3 obj_color, t_vec3 normal)
{
    // Start with ambient lighting
    t_ambient_light *amb = scene->ambient_light;
    t_vec3 amb_color = {amb->color_ambient_light->r / 255.0, 
                        amb->color_ambient_light->g / 255.0, 
                        amb->color_ambient_light->b / 255.0};
    t_vec3 final_color = vec_mult(obj_color, vec_scale(amb_color, amb->bright_ambient_light));
    
    // Material properties
    double shininess = 32.0;      // Shininess factor (higher = sharper highlights)
    double specular_strength = 0.5; // How strong specular highlights are
    
    // Process each light source
    t_light *current_light = scene->light;
    while (current_light)
    {
        t_vec3 light_pos = {current_light->coor_light->x, 
                            current_light->coor_light->y, 
                            current_light->coor_light->z};
        
        // Check if point is in shadow
        if (is_in_shadow(scene, hit_rec->p, light_pos))
        {
            current_light = current_light->next;
            continue; // Skip this light, it's blocked
        }
        
        t_vec3 to_light = vec_sub(light_pos, hit_rec->p);
        double light_distance = vec_length(to_light);
        t_vec3 light_dir = vec_normalize(to_light);
        
        // Diffuse lighting
        double diff_factor = fmax(vec_dot(normal, light_dir), 0.0);
        
        // Light attenuation (optional: light gets weaker with distance)
        double attenuation = 1.0 / (1.0 + 0.01 * light_distance + 0.001 * light_distance * light_distance);
        // double attenuation = 1;
        
        t_vec3 light_color = {current_light->color_light->r / 255.0, 
                              current_light->color_light->g / 255.0, 
                              current_light->color_light->b / 255.0};
        double light_intensity = current_light->bright_light;
        
        // Diffuse contribution
        t_vec3 diffuse = vec_mult(obj_color, 
                                  vec_scale(light_color, diff_factor * light_intensity * attenuation));
        
        // Specular contribution (white highlights)
        t_vec3 specular = calculate_specular(light_dir, normal, view_dir, 
                                             light_color, light_intensity * attenuation, 
                                             shininess);
        specular = vec_scale(specular, specular_strength);
        
        // Add diffuse and specular to final color
        final_color = vec_add(final_color, diffuse);
        final_color = vec_add(final_color, specular);
        
        current_light = current_light->next;
    }
    
    return final_color;
}

/******************************************************************************* */
// Add this function to mlx_start.c after intersect_sphere

double intersect_cylinder(t_ray *ray, t_cylinder *cylinder, t_hit_record *rec)
{
    t_vec3 center = *cylinder->coor_cylinder;
    t_vec3 axis = vec_normalize(*cylinder->vector_cylinder);
    double radius = cylinder->diameter_cylinder / 2.0;
    double height = cylinder->height_cylinder;
    
    // Vector from cylinder base to ray origin
    t_vec3 oc = vec_sub(ray->origin, center);
    
    // Project ray direction and oc onto plane perpendicular to cylinder axis
    t_vec3 ray_perp = vec_sub(ray->direction, vec_scale(axis, vec_dot(ray->direction, axis)));
    t_vec3 oc_perp = vec_sub(oc, vec_scale(axis, vec_dot(oc, axis)));
    
    // Solve quadratic equation for infinite cylinder
    double a = vec_dot(ray_perp, ray_perp);
    double b = 2.0 * vec_dot(oc_perp, ray_perp);
    double c = vec_dot(oc_perp, oc_perp) - radius * radius;
    
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0.0)
        return INFINITY;
    
    double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    
    double closest_t = INFINITY;
    int hit_body = 0;
    
    // Check both intersection points for body
    for (int i = 0; i < 2; i++)
    {
        double t = (i == 0) ? t1 : t2;
        if (t < 0) continue;
        
        t_vec3 hit_point = vec_add(ray->origin, vec_scale(ray->direction, t));
        t_vec3 to_hit = vec_sub(hit_point, center);
        double proj = vec_dot(to_hit, axis);
        
        // Check if hit is within cylinder height
        if (proj >= 0 && proj <= height && t < closest_t)
        {
            closest_t = t;
            hit_body = 1;
            
            if (rec)
            {
                rec->t = t;
                rec->p = hit_point;
                
                // Normal perpendicular to axis
                t_vec3 proj_vec = vec_scale(axis, proj);
                t_vec3 center_at_height = vec_add(center, proj_vec);
                rec->normal = vec_normalize(vec_sub(hit_point, center_at_height));
                rec->color = *cylinder->color_cylinder;
                
                // UV mapping for cylinder body
                double theta = atan2(rec->normal.z, rec->normal.x);
                rec->u = (theta + M_PI) / (2.0 * M_PI);
                rec->v = proj / height;
                rec->hit_object = cylinder;
            }
        }
    }
    
    // Check top cap
    double denom_top = vec_dot(ray->direction, axis);
    if (fabs(denom_top) > 1e-6)
    {
        t_vec3 cap_center = vec_add(center, vec_scale(axis, height));
        t_vec3 to_cap = vec_sub(cap_center, ray->origin);
        double t = vec_dot(to_cap, axis) / denom_top;
        
        if (t > 0)
        {
            t_vec3 hit_point = vec_add(ray->origin, vec_scale(ray->direction, t));
            t_vec3 to_hit = vec_sub(hit_point, cap_center);
            
            if (vec_length(to_hit) <= radius && t < closest_t)
            {
                closest_t = t;
                if (rec)
                {
                    rec->t = t;
                    rec->p = hit_point;
                    rec->normal = axis;
                    rec->color = *cylinder->color_cylinder;
                    rec->u = (to_hit.x / radius + 1.0) * 0.5;
                    rec->v = (to_hit.z / radius + 1.0) * 0.5;
                    rec->hit_object = cylinder;
                }
            }
        }
    }
    
    // Check bottom cap
    double denom_bottom = vec_dot(ray->direction, axis);
    if (fabs(denom_bottom) > 1e-6)
    {
        t_vec3 to_cap = vec_sub(center, ray->origin);
        double t = vec_dot(to_cap, axis) / denom_bottom;
        
        if (t > 0)
        {
            t_vec3 hit_point = vec_add(ray->origin, vec_scale(ray->direction, t));
            t_vec3 to_hit = vec_sub(hit_point, center);
            
            if (vec_length(to_hit) <= radius && t < closest_t)
            {
                closest_t = t;
                if (rec)
                {
                    rec->t = t;
                    rec->p = hit_point;
                    rec->normal = vec_scale(axis, -1.0);
                    rec->color = *cylinder->color_cylinder;
                    rec->u = (to_hit.x / radius + 1.0) * 0.5;
                    rec->v = (to_hit.z / radius + 1.0) * 0.5;
                    rec->hit_object = cylinder;
                }
            }
        }
    }
    
    return closest_t;
}
/******************************************************************************* */
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
        double scale = 9; // Tiling scale (adjust for texture frequency)
        t_vec3 to_hit = vec_sub(rec->p, plane_point); // Vector from plane origin to hit point
        t_vec3 arbitrary = (fabs(plane_normal.x) > 0.9) ? (t_vec3){0,1,0} : (t_vec3){1,0,0}; // Avoid parallel to normal
        t_vec3 tangent_u = vec_normalize(vec_cross(plane_normal, arbitrary));
        t_vec3 tangent_v = vec_cross(plane_normal, tangent_u);
        
        rec->u = fmod(vec_dot(to_hit, tangent_u) / scale, 1.0);
        if (rec->u < 0) 
        rec->u += 1.0; // Ensure [0,1]
        rec->v = fmod(vec_dot(to_hit, tangent_v) / scale, 1.0);
        if (rec->v < 0)
        rec->v += 1.0;
        rec->hit_object = plane;
        
        // printf("Plane UV at (%f, %f, %f): u=%f, v=%f\n", rec->p.x, rec->p.y, rec->p.z, rec->u, rec->v);
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

void    set_scene(t_scene *scene, bool flag)
{
    static void *holder = NULL; 
    if(!flag)
    {
        puts("--------------------------------------stored ---------------------------------\n");
        holder = scene->cleaner;
    }
    else
    {
        ((t_cleanup *)holder)->flag_input = true;
        exit_error("closing", "window", ((t_cleanup *)holder));
    }
}

void    close_window(mlx_key_data_t keydata, void* param)
{
    if(keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        // printf("window closed !!!!!\n");
        // t_scene *scene;
        set_scene(NULL, true);
        // exit_error("window", "closed", cleaner);
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
    // print_camera_debug(cam);
    // if (scene->plane)
    //     print_plane_debug(scene->plane);

     y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            // Use 2x2 supersampling (4 samples per pixel)
            t_vec3 accumulated_color = {0, 0, 0};
            int samples = 0;
            
            // Sample at 4 sub-pixel positions in a grid
            for (int sy = 0; sy < 2; sy++)
            {
                for (int sx = 0; sx < 2; sx++)
                {
                    // Offset by 0.25 and 0.75 for each dimension
                    double offset_x = (sx * 0.5 + 0.25);
                    double offset_y = (sy * 0.5 + 0.25);
                    
                    double u = ((double)x + offset_x) / (WIDTH - 1);
                    double v = 1.0 - ((double)y + offset_y) / (HEIGHT - 1);

                    ray = get_ray(cam, u, v);

                    double closest_t = INFINITY;
                    t_hit_record hit_rec;
                    int hit = 0;

                    // Test planes
                    t_plane *curr_plane = scene->plane;
                    while (curr_plane)
                    {
                        double t = intersect_plane(&ray, curr_plane, NULL);
                        if (t < closest_t)
                        {
                            closest_t = t;
                            intersect_plane(&ray, curr_plane, &hit_rec);
                            hit = 1;
                        }
                        curr_plane = curr_plane->next;
                    }

                    // Test spheres
                    t_sphere *curr_sphere = scene->sphere;
                    while (curr_sphere)
                    {
                        double t = intersect_sphere(&ray, curr_sphere, NULL);
                        if (t < closest_t)
                        {
                            closest_t = t;
                            intersect_sphere(&ray, curr_sphere, &hit_rec);
                            hit = 1;
                        }
                        curr_sphere = curr_sphere->next;
                    }
                    
                    // Test cylinders
                    t_cylinder *curr_cylinder = scene->cylinder;
                    while (curr_cylinder)
                    {
                        double t = intersect_cylinder(&ray, curr_cylinder, NULL);
                        if (t < closest_t)
                        {
                            closest_t = t;
                            intersect_cylinder(&ray, curr_cylinder, &hit_rec);
                            hit = 1;
                        }
                        curr_cylinder = curr_cylinder->next;
                    }
                    
                    t_vec3 sample_color_t = {0, 0, 0};
                    
                    if (hit)
                    {
                        t_vec3 hit_point = hit_rec.p;
                        t_vec3 normal = hit_rec.normal;
                        t_vec3 obj_color = {hit_rec.color.r / 255.0, 
                                            hit_rec.color.g / 255.0, 
                                            hit_rec.color.b / 255.0};
                        
                        // Check for bump mapping
                        t_texture *bump_tex = NULL;
                        if (hit_rec.hit_object)
                        {
                            t_sphere *test_sphere = scene->sphere;
                            while (test_sphere)
                            {
                                if (test_sphere == (t_sphere *)hit_rec.hit_object && test_sphere->flag_bump)
                                {
                                    bump_tex = test_sphere->bump_texture;
                                    if (test_sphere->bump_texture)
                                        obj_color = sample_color(test_sphere->bump_texture, hit_rec.u, hit_rec.v);
                                    break;
                                }
                                test_sphere = test_sphere->next;
                            }
                            
                            t_plane *test_plane = scene->plane;
                            while (test_plane)
                            {
                                if (test_plane == (t_plane *)hit_rec.hit_object && test_plane->flag_bump)
                                {
                                    bump_tex = test_plane->bump_texture;
                                    if (test_plane->bump_texture)
                                        obj_color = sample_color(test_plane->bump_texture, hit_rec.u, hit_rec.v);
                                    break;
                                }
                                test_plane = test_plane->next;
                            }
                            t_cylinder *test_cylinder = scene->cylinder;
                            while (test_cylinder)
                            {
                                if (test_cylinder == (t_cylinder *)hit_rec.hit_object && test_cylinder->flag_bump)
                                {
                                    bump_tex = test_cylinder->bump_texture;
                                    if (test_cylinder->bump_texture)
                                        obj_color = sample_color(test_cylinder->bump_texture, hit_rec.u, hit_rec.v);
                                    break;
                                }
                                test_cylinder = test_cylinder->next;
                            }
                            
                            if (bump_tex)
                            {
                                double offset = 0.0001;
                                double h = sample_height(bump_tex, hit_rec.u, hit_rec.v);
                                double hu = sample_height(bump_tex, hit_rec.u + offset, hit_rec.v);
                                double hv = sample_height(bump_tex, hit_rec.u, hit_rec.v + offset);
                                
                                double dHdu = (hu - h) / offset;
                                double dHdv = (hv - h) / offset;
                                
                                t_vec3 arbitrary = (fabs(normal.x) > 0.9) ? (t_vec3){0,1,0} : (t_vec3){1,0,0};
                                t_vec3 tangent_u = vec_normalize(vec_cross(normal, arbitrary));
                                t_vec3 tangent_v = vec_cross(normal, tangent_u);
                                
                                normal = vec_normalize(vec_add(normal, 
                                                              vec_add(vec_scale(tangent_u, dHdu * bump_tex->scale),
                                                                      vec_scale(tangent_v, dHdv * bump_tex->scale))));
                            }
                        }
                        
                        // Calculate view direction (from hit point to camera)
                        t_vec3 view_dir = vec_normalize(vec_sub(ray.origin, hit_point));
                        
                        // Use new lighting system with shadows and specular
                        sample_color_t = calculate_lighting(scene, &hit_rec, view_dir, obj_color, normal);
                    }
                    
                    accumulated_color = vec_add(accumulated_color, sample_color_t);
                    samples++;
                }
            }
            
            // Average all 4 samples
            accumulated_color = vec_scale(accumulated_color, 1.0 / samples);
            
            // Clamp
            accumulated_color.x = fmin(1.0, fmax(0.0, accumulated_color.x));
            accumulated_color.y = fmin(1.0, fmax(0.0, accumulated_color.y));
            accumulated_color.z = fmin(1.0, fmax(0.0, accumulated_color.z));
            
            t_color final_color;
            final_color.r = (int)(accumulated_color.x * 255);
            final_color.g = (int)(accumulated_color.y * 255);
            final_color.b = (int)(accumulated_color.z * 255);
            
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
    // mlx_delete_texture()
    mlx_delete_image(mlx, img);
    // mlx_terminate(mlx);
}
