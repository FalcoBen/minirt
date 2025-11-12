#include "../../MiniRt.h"

// bool is_shadowed(t_world *world, t_tuple point)
// {
//     t_tuple v;
//     ld distance;
//     t_stack_ray r;
//     t_stack_intersections intersections;
//     t_inters h;

//     s_sub_t(&v, *(world->light->position), point);
//     distance = magn(&v);
//     s_vec_norm(&v);
//     r = s_ray_constrctor(&point, &v);
//     s_intersect_world(&intersections, *world, r);
//     h = s_hit(intersections);
//     if (h.existence && h.inters_value < distance)
//         return true;
//     return false;
// }
 

// OLD signature (single light in world):
// bool is_shadowed(t_world *world, t_tuple point)

// NEW signature (specific light parameter):
// bool is_shadowed(t_world *world, t_tuple point, t_light *light)
// {
//     // Calculate direction from point to THIS light
//     t_tuple light_direction;
//     s_sub_t(&light_direction, *(light->position), point);
    
//     ld distance = magn(&light_direction);
//     s_vec_norm(&light_direction);
    
//     // Create shadow ray
//     t_stack_ray shadow_ray;
//     shadow_ray.point = point;
//     shadow_ray.vect = light_direction;
    
//     // Check for intersections
//     t_stack_intersections xs;
//     s_intersect_world(&xs, *world, shadow_ray);
    
//     t_inters h = s_hit(xs);
    
//     // If hit exists and is between point and light, it's in shadow
//     if (h.existence && h.inters_value < distance)
//         return true;
    
//     return false;
// }

// shadow.c
bool is_shadowed(t_world *world, t_tuple point, t_light *light) // fahd
{
    t_tuple light_direction;
    s_sub_t(&light_direction, *(light->position), point);
    
    ld distance = magn(&light_direction);
    s_vec_norm(&light_direction);
    
    t_stack_ray shadow_ray;
    shadow_ray.point = point;
    shadow_ray.vect = light_direction;
    
    // ✅ Initialize properly
    t_stack_intersections xs;
    xs.count = 0;
    xs.existence = false;
    xs.world_inters_list = NULL;
    
    s_intersect_world(&xs, *world, shadow_ray);
    
    if (!xs.existence || xs.count == 0)
    {
        if (xs.world_inters_list)
            free(xs.world_inters_list);
        return false;
    }
    
    t_inters h = s_hit(xs);
    
    bool shadowed = false;
    
    // ✅ CRITICAL: Check for SHADOW_EPSILON and distance
    if (h.existence && 
        h.inters_value > SHADOW_EPSILON && 
        h.inters_value < (distance - SHADOW_EPSILON))
    {
        shadowed = true;
    }
    
    if (xs.world_inters_list)
        free(xs.world_inters_list);
    
    return shadowed;
}