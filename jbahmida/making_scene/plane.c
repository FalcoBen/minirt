#include "../../MiniRt.h"

// void plane(t_plane **plane, t_tuple *origin, t_tuple *vect)
// {
//     if (!plane)
//         return;
//     static int id = 0;
//     (*plane) = alloc(sizeof(t_plane), false);
//     (*plane)->origin = origin;
//     (*plane)->vect = vect;
//     (*plane)->transform = identity_matrix(4, 4);
//     (*plane)->inv = identity_matrix(4, 4);
//     (*plane)->id = id;
//     (*plane)->material = material();
//     id++;
// }

// // Plane intersection using origin + vect (no transformation needed!)
// void plane_intersct(t_stack_intersections *xs, t_plane *plane, t_stack_ray *ray)
// {
//     xs->count = 0;
//     xs->existence = false;
    
//     // Plane equation: dot(N, P - P0) = 0
//     // Where N = normal (plane->vect), P = point on plane, P0 = plane->origin
    
//     // Ray equation: P = ray_origin + t * ray_direction
//     // Solving for t:
//     // t = dot(N, P0 - ray_origin) / dot(N, ray_direction)
    
//     t_tuple normal = *(plane->vect);
//     s_vec_norm(&normal);  // Ensure it's normalized
    
//     // Check if ray is parallel to plane
//     ld denominator = dot_product(&normal, &ray->vect);
    
//     if (fabsf(denominator) < FLT_EPSILON)
//     {
//         // Ray is parallel to plane (no intersection)
//         xs->world_inters_list = NULL;
//         return;
//     }
    
//     // Calculate t
//     t_tuple origin_to_ray;
//     s_sub_t(&origin_to_ray, ray->point, *(plane->origin));
    
//     ld numerator = -dot_product(&normal, &origin_to_ray);
//     ld t = numerator / denominator;
    
//     // Create intersection
//     xs->inters_list[0].type = T_PLAN;
//     xs->inters_list[0].inters_value = t;
//     xs->inters_list[0].sphere = NULL;
//     xs->inters_list[0].cylinder = NULL;
//     xs->inters_list[0].cone = NULL;
//     xs->inters_list[0].plane = plane;
//     xs->inters_list[0].existence = true;
//     xs->count = 1;
//     xs->existence = true;
// }

// void s_plane_normal(t_tuple *norm, t_plane *plane, t_tuple *point)
// {
//     (void)point;  // Point not used - normal is constant
    
//     // Return the plane's normal vector (already in world space)
//     *norm = *(plane->vect);
//     norm->w = 0;  // Ensure it's a vector
    
//     // Normalize
//     s_vec_norm(norm);
// }

// int number_of_all_objects(t_scene *scene)
// {
//     int counter = 0;
//     t_sphere_fb *sp = scene->sphere;
//     t_plane_fb *pl = scene->plane;
//     t_cylinder_fb *cy = scene->cylinder;
//     t_cone_fb *co = scene->cone;
//     while(sp)
//     {
//         counter++;
//         sp = sp->next;
//     }
//     while(pl)
//     {
//         counter++;
//         pl = pl->next;
//     }
//     while(cy)
//     {
//         counter++;
//         cy = cy->next;
//     }
//     while(co)
//     {
//         counter++;
//         co = co->next;
//     }
//     // scene->obj_num = counter;
//     return counter;
// }

void plane(t_plane **plane, t_tuple *origin, t_tuple *vect)
{
    if (!plane)
        return;
    static int id = 0;
    
    (*plane) = alloc(sizeof(t_plane), false);
    
    // Store the identity transform initially
    (*plane)->transform = identity_matrix(4, 4);
    (*plane)->inv = identity_matrix(4, 4);
    (*plane)->id = id;
    (*plane)->material = material();
    
    // We'll compute the transformation from origin and normal
    // Store these temporarily to build the transform
    (*plane)->origin = origin;
    (*plane)->vect = vect;
    
    id++;
}

// ✅ NEW: Helper to create plane transformation from origin and normal
t_matrix *create_plane_transform(t_tuple *origin, t_tuple *normal)
{
    // Normalize the normal
    t_tuple normalized_normal = *normal;
    s_vec_norm(&normalized_normal);
    
    // Default plane normal in object space is (0, 1, 0) pointing up
    t_tuple default_y = {0, 1, 0, 0};
    
    // If normal is already pointing up, just translate
    ld dot = dot_product(&default_y, &normalized_normal);
    
    t_matrix *rotation;
    
    if (fabsf(dot - 1.0) < 0.001)
    {
        // Normal is (0,1,0) - no rotation needed
        rotation = identity_matrix(4, 4);
    }
    else if (fabsf(dot + 1.0) < 0.001)
    {
        // Normal is (0,-1,0) - rotate 180° around X
        rotation = rotation_x(M_PI);
    }
    else
    {
        // General case: rotate from (0,1,0) to the desired normal
        // We need to find the rotation axis and angle
        
        // Rotation axis is perpendicular to both normals
        t_tuple axis = *cross_product(&default_y, &normalized_normal);
        s_vec_norm(&axis);
        
        // Rotation angle
        ld angle = acos(dot);
        
        // For simplicity, handle specific cases:
        if (fabsf(normalized_normal.x) > 0.999)
        {
            // Normal points along X-axis
            rotation = rotation_z(normalized_normal.x > 0 ? -M_PI/2 : M_PI/2);
        }
        else if (fabsf(normalized_normal.z) > 0.999)
        {
            // Normal points along Z-axis
            rotation = rotation_x(normalized_normal.z > 0 ? M_PI/2 : -M_PI/2);
        }
        else
        {
            // TODO: Implement general rotation using axis and angle
            // For now, use identity (you'll need to implement axis-angle rotation)
            rotation = identity_matrix(4, 4);
        }
    }
    
    // Translate to the origin point
    t_matrix *trans = translation(origin->x, origin->y, origin->z);
    
    // Combine: Translation * Rotation
    t_matrix *result = matrix_multi(trans, rotation);
    
    copy_matrix_contant(result);
    return result;
}

// ✅ FIXED: Plane intersection now works like other objects
void plane_intersct(t_stack_intersections *xs, t_plane *plane, t_stack_ray *ray)
{
    xs->count = 0;
    xs->existence = false;
    
    // ✅ CRITICAL: Transform ray to plane's object space!
    t_stack_ray local_ray = s_transform(ray, plane->inv);
    
    // In object space, the plane is at y=0 with normal (0,1,0)
    // Plane equation: y = 0
    // Ray equation: P = origin + t * direction
    // At intersection: origin.y + t * direction.y = 0
    // So: t = -origin.y / direction.y
    
    // Check if ray is parallel to plane (direction.y ≈ 0)
    if (fabsf(local_ray.vect.y) < FLT_EPSILON)
    {
        // Ray is parallel to plane (no intersection)
        xs->world_inters_list = NULL;
        return;
    }
    
    // Calculate t
    ld t = -local_ray.point.y / local_ray.vect.y;
    
    // Create intersection
    xs->inters_list[0].type = T_PLAN;
    xs->inters_list[0].inters_value = t;
    xs->inters_list[0].sphere = NULL;
    xs->inters_list[0].cylinder = NULL;
    xs->inters_list[0].cone = NULL;
    xs->inters_list[0].plane = plane;
    xs->inters_list[0].existence = true;
    xs->count = 1;
    xs->existence = true;
}

// // ✅ FIXED: Plane normal now works in object space
// void s_plane_normal(t_tuple *norm, t_plane *plane, t_tuple *point)
// {
//     (void)point;  // Point not used - normal is constant in object space
    
//     // In object space, plane normal is always (0, 1, 0)
//     t_tuple local_normal = {0, 1, 0, 0};
    
//     // ✅ Transform normal back to world space
//     t_matrix transpose = s_matrix_transpos(plane->inv);
//     t_matrix world_normal_matrix = s_matrix_tuple(&transpose, &local_normal);
//     *norm = s_matrix_to_tuple(&world_normal_matrix);
//     norm->w = 0;
//     s_vec_norm(norm);
// }


void s_plane_normal(t_tuple *norm, t_plane *plane, t_tuple *point)
{
    // In object space, plane normal is always (0, 1, 0)
    t_tuple local_normal = {0, 1, 0, 0};
    
    //  Apply bump mapping if enabled
    if (plane->material->has_bump_map && plane->material->bump_map)
    {
        ld u, v;
        plane_uv(point, plane, &u, &v);
        
        if (plane->material->bump_map->type == 1)
        {
            // Checkerboard bump - create height variation
            ld freq = 1.0 / plane->material->bump_map->scale;
            u = fmod(u * freq, 1.0);
            v = fmod(v * freq, 1.0);
            if (u < 0) u += 1.0;
            if (v < 0) v += 1.0;
            
            int iu = (int)(u);
            int iv = (int)(v);
            
            // Create bump perturbation based on checkerboard pattern
            ld bump_height = ((iu + iv) % 2 == 0) ? 0.0 : 1.0;
            
            // Calculate finite differences for gradient
            ld eps = 0.01;
            int iu_next = (int)(u + eps);
            int iv_next = (int)(v + eps);
            
            ld height_u = ((iu_next + iv) % 2 == 0) ? 0.0 : 1.0;
            ld height_v = ((iu + iv_next) % 2 == 0) ? 0.0 : 1.0;
            
            // Compute gradient (how height changes)
            ld du = (height_u - bump_height) * plane->material->bump_strength * 2.0;
            ld dv = (height_v - bump_height) * plane->material->bump_strength * 2.0;
            
            // Perturb normal in object space
            local_normal.x += du;
            local_normal.z += dv;
        }
        else if (plane->material->bump_map->type == 2 && plane->material->bump_map->image)
        {
            // Image-based bump mapping
            ld height = sample_height(plane->material->bump_map, u, v);
            
            // Calculate finite differences
            ld eps = 0.001;
            ld height_u = sample_height(plane->material->bump_map, u + eps, v);
            ld height_v = sample_height(plane->material->bump_map, u, v + eps);
            
            // Compute gradient
            ld du = (height_u - height) * plane->material->bump_strength * 10.0;
            ld dv = (height_v - height) * plane->material->bump_strength * 10.0;
            
            // Perturb normal
            local_normal.x += du;
            local_normal.z += dv;
        }
    }
    
    // Transform normal to world space
    t_matrix transpose = s_matrix_transpos(plane->inv);
    t_matrix world_normal_matrix = s_matrix_tuple(&transpose, &local_normal);
    *norm = s_matrix_to_tuple(&world_normal_matrix);
    norm->w = 0;
    s_vec_norm(norm);  // Normalize after perturbation
}


int number_of_all_objects(t_scene *scene)
{
    int counter = 0;
    t_sphere_fb *sp = scene->sphere;
    t_plane_fb *pl = scene->plane;
    t_cylinder_fb *cy = scene->cylinder;
    t_cone_fb *co = scene->cone;
    while(sp)
    {
        counter++;
        sp = sp->next;
    }
    while(pl)
    {
        counter++;
        pl = pl->next;
    }
    while(cy)
    {
        counter++;
        cy = cy->next;
    }
    while(co)
    {
        counter++;
        co = co->next;
    }
    return counter;
}