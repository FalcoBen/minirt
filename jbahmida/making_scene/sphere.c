#include "../../MiniRt.h"


void cone_uv(t_tuple *point, t_cone *cone, ld *u, ld *v)
{
    // Transform point to object space
    t_matrix point_matrix = s_matrix_tuple(cone->inv, point);
    t_tuple local_point = s_matrix_to_tuple(&point_matrix);

    // U: angle around Y axis
    ld theta = atan2(local_point.x, local_point.z);
    *u = 1.0 - (theta + M_PI) / (2.0 * M_PI);

    // V: height along Y axis, mapped from [minimum, maximum] to [0, 1]
    ld range = cone->maximum - cone->minimum;
    if (fabs(range) > 0.0001)
        *v = (local_point.y - cone->minimum) / range;
    else
        *v = 0.5;

    // Wrap U to [0, 1]
    *u = fmod(*u, 1.0);
    if (*u < 0) *u += 1.0;

    // Clamp V to [0, 1]
    *v = fmax(0.0, fmin(1.0, *v));
}


// SPHERE UV MAPPING with proper scaling
void sphere_uv(t_tuple *point, t_sphere *sphere, ld *u, ld *v)
{
    // Transform to object space first
    t_matrix point_matrix = s_matrix_tuple(sphere->inv, point);
    t_tuple local_point = s_matrix_to_tuple(&point_matrix);
    
    // Convert to spherical coordinates
    ld phi = atan2(local_point.z, local_point.x);
    ld theta = asin(local_point.y);
    
    *u = 1 - (phi + M_PI) / (2 * M_PI);
    *v = (theta + M_PI/2) / M_PI;
    
    // Apply consistent scaling
    if(sphere->material->has_bump_map && sphere->material->bump_map && sphere->material->bump_map->type == 1)
    {
        *u *= 2.0;  // Adjust this value to match plane scale
        *v *= 2.0;
    }
}

// void plane_uv(t_tuple *point, t_plane *plane, ld *u, ld *v)
// {
//     // Transform point to plane's object space
//     t_matrix point_matrix = s_matrix_tuple(plane->inv, point);
//     t_tuple local_point = s_matrix_to_tuple(&point_matrix);
    
//     // CRITICAL: For walls, we want UV to span 0-1 across the wall
//     // NOT repeat multiple times
    
//     // Check if this is a checkerboard (should tile) or image texture (shouldn't tile)
//     bool should_tile = false;
//     if (plane->material->has_color_texture && 
//         plane->material->color_texture && 
//         plane->material->color_texture->type == 1)
//     {
//         should_tile = true;  // Checkerboard tiles
//     }
    
//     if (should_tile)
//     {
//         // For checkerboard - allow tiling with scale
//         ld scale = plane->material->color_texture->scale;
//         if (scale < 0.01) scale = 1.0;  // Prevent division by zero
        
//         *u = local_point.x / scale;
//         *v = local_point.z / scale;
//     }
//     else
//     {
//         // For image textures - map to [0,1] without tiling
//         // Normalize based on expected wall dimensions
//         // Assuming walls are roughly 10 units wide
//         ld wall_width = 12.0;
        
//         *u = (local_point.x + wall_width/2.0) / wall_width;
//         *v = (local_point.z + wall_width/2.0) / wall_width;
        
//         // Clamp to [0,1] instead of wrapping
//         *u = fmax(0.0, fmin(1.0, *u));
//         *v = fmax(0.0, fmin(1.0, *v));
        
//         return;  // Don't wrap for image textures
//     }
    
//     // Only wrap for tiling textures
//     *u = fmod(*u, 1.0);
//     *v = fmod(*v, 1.0);
//     if (*u < 0) *u += 1.0;
//     if (*v < 0) *v += 1.0;
// }



/* --------------------------------------------------------------
   CHECKERBOARD UV (type == 1)
   -------------------------------------------------------------- */
static void plane_uv_checker(t_tuple *point, t_plane *plane, ld *u, ld *v)
{
    /* 1. point → object space */
    t_matrix m   = s_matrix_tuple(plane->inv, point);
    t_tuple  loc = s_matrix_to_tuple(&m);

    /* 2. scale – honour user value, otherwise a visible default */
    ld scale = 10.0;                                   /* ≈ 0.5×0.5 squares */
    // if (plane->material->has_color_texture &&
    //     plane->material->color_texture &&
    //     plane->material->color_texture->type == 1 &&
    //     plane->material->color_texture->scale > 0.001)
    // {
    //     scale = plane->material->color_texture->scale;
    // }

    /* 3. raw UV (still infinite) */
    *u = loc.x / scale;
    *v = loc.z / scale;

    /* 4. wrap forever – checkerboard tiles */
    *u = fmod(*u, 1.0);
    *v = fmod(*v, 1.0);
    if (*u < 0) *u += 1.0;
    if (*v < 0) *v += 1.0;
}

/* --------------------------------------------------------------
   IMAGE UV (type == 2)
   -------------------------------------------------------------- */
static void plane_uv_image(t_tuple *point, t_plane *plane, ld *u, ld *v)
{
    t_matrix m   = s_matrix_tuple(plane->inv, point);
    t_tuple  loc = s_matrix_to_tuple(&m);

    /* Image textures are **not** scaled by the texture’s .scale field.
       They are mapped **once** over the whole plane (or a user-defined
       rectangle if you ever add width/height). */
    ld scale = 12.0;                     /* same default as checkerboard */
    *u = loc.x / scale;
    *v = loc.z / scale;

    /* Show the image once → clamp to [0,1] */
    *u = *u * 0.5 + 0.5;
    *v = *v * 0.5 + 0.5;
    *u = fmax(0.0, fmin(1.0, *u));
    *v = fmax(0.0, fmin(1.0, *v));
}

/* --------------------------------------------------------------
   PUBLIC dispatcher – keep the old name for backward compatibility
   -------------------------------------------------------------- */
void plane_uv(t_tuple *point, t_plane *plane, ld *u, ld *v)
{
    if (plane->material->has_color_texture &&
        plane->material->color_texture &&
        plane->material->color_texture->type == 1)
    {
        plane_uv_checker(point, plane, u, v);
    }
    else if (plane->material->has_bump_map &&
             plane->material->bump_map &&
             plane->material->bump_map->type == 2 &&
             plane->material->bump_map->image)
    {
        plane_uv_image(point, plane, u, v);
    }
    else
    {
        /* fallback – should never be hit for textured objects */
        *u = 0.0; *v = 0.0;
    }
}

void cylinder_uv(t_tuple *point, t_cylinder *cylinder, ld *u, ld *v)
{
    // Transform point to object space
    t_matrix point_matrix = s_matrix_tuple(cylinder->inv, point);
    t_tuple local_point = s_matrix_to_tuple(&point_matrix);
    
    // Calculate U (wraps around cylinder 0 to 1)
    ld theta = atan2(local_point.x, local_point.z);
    *u = 1.0 - (theta + M_PI) / (2.0 * M_PI);
    
    // ✅ FIX: Handle V coordinate properly
    // Check for infinite bounds
    if (isinf(cylinder->minimum) || isinf(cylinder->maximum))
    {
        // For infinite cylinders, use absolute Y position with scaling
        *v = local_point.y * 0.1;  // Scale factor to make pattern visible
        
        // Wrap to [0,1] range
        *v = fmod(*v, 1.0);
        if (*v < 0) *v += 1.0;
    }
    else
    {
        // For finite cylinders, map Y from [minimum, maximum] to [0, 1]
        ld range = cylinder->maximum - cylinder->minimum;
        if (fabs(range) > 0.0001)  // Avoid division by zero
            *v = (local_point.y - cylinder->minimum) / range;
        else
            *v = 0.5;
    }
    
    // Wrap U to [0, 1]
    *u = fmod(*u, 1.0);
    if (*u < 0) *u += 1.0;
    
    // Ensure V is in valid range
    if (isnan(*v) || isinf(*v))
        *v = 0.0;
    
    // // Debug output
    // printf("CYLINDER UV: local point(%.3f,%.3f,%.3f)\n", 
    //        local_point.x, local_point.y, local_point.z);
    // printf("CYLINDER UV: min=%.3f max=%.3f range=%.3f\n",
    //        cylinder->minimum, cylinder->maximum, 
    //        cylinder->maximum - cylinder->minimum);
    // printf("CYLINDER UV: final u=%.3f, v=%.3f\n", *u, *v);
}


void sphere(t_sphere **sphere, t_matrix *tran, bool flag)
{
    if (!sphere)
        return ;
    static int id = 0;
    if (flag)
    {
        (*sphere)->transform = tran;
        (*sphere)->inv = matrix_inverte((*sphere)->transform);
        // (*sphere)->origin = matrix_to_tuple(matrix_tuple((*sphere)->inv, (*sphere)->origin));        
        return ;
    }
    (*sphere) = alloc(sizeof(t_sphere), false);
    (*sphere)->transform = identity_matrix(4, 4);
    (*sphere)->inv = identity_matrix(4, 4);
    (*sphere)->origin = tuple_constractor(0,0,0,1);
    (*sphere)->radius = 1;
    (*sphere)->id = id;
    (*sphere)->material = material();
    // *sphere = s;
    id++;
}


void s_intersect(t_stack_intersections *xs, t_stack_ray ray, t_sphere *s)
{
    ld a, b, c, delta;
    t_stack_ray local_ray = s_transform(&ray, s->inv);
    a = dot_product(&local_ray.vect, &local_ray.vect);
    b = 2 * dot_product(&local_ray.point, &local_ray.vect);
    c = dot_product(&local_ray.point, &local_ray.point) - 1;
    delta = pow(b, 2) - 4 * a * c;
    if (delta < 0.00)
    {
        xs->count = 0;
        xs->existence = false;
        xs->world_inters_list = NULL;
        return;
    }
    // xs->count = (delta > 0.0001) ? 2 : (delta > -0.0001) ? 1 : 0;
    xs->existence = true;
    if (delta < INTER_EPSILON)
    {
        xs->count = 1;
        xs->inters_list[0].inters_value = (-b) / (2 * a);
        xs->inters_list[0].sphere = s;
        xs->inters_list[0].cylinder = NULL;
        xs->inters_list[0].cone = NULL;
        xs->inters_list[0].plane = NULL;
        xs->inters_list[0].existence = true;
        xs->inters_list[0].type = T_SPHERE;
    }
    else
    {
        xs->count = 2;
        ld sqrt_delta = sqrtl(delta);
        xs->inters_list[0].inters_value = (-b - sqrt_delta) / (2 * a);
        xs->inters_list[1].inters_value = (-b + sqrt_delta) / (2 * a);
        xs->inters_list[0].sphere = xs->inters_list[1].sphere = s;
        xs->inters_list[0].cylinder = xs->inters_list[1].cylinder = NULL;
        xs->inters_list[0].cone = xs->inters_list[1].cone = NULL;
        xs->inters_list[0].plane = xs->inters_list[1].plane = NULL;
        xs->inters_list[0].existence = xs->inters_list[1].existence = true;
        xs->inters_list[0].type = xs->inters_list[1].type = T_SPHERE;
    }
}

// void s_normal_at(t_tuple *normal, t_sphere *s, t_tuple *point)
// {
//     copy_matrix_contant(s->inv);
//     t_matrix m = s_matrix_tuple(s->inv, point);
//     t_tuple obj_point = s_matrix_to_tuple(&m);
//     t_tuple obj_normal = (t_tuple){0,0,0,0};
//     obj_normal = (t_tuple){0, 0, 0, 0};
//     s_sub_t(&obj_normal, obj_point, *(s->origin));
//     t_matrix inv_transpose = s_matrix_transpos(s->inv);
//     t_matrix m1 = s_matrix_tuple(&inv_transpose, &obj_normal);
//     t_tuple w_normal = s_matrix_to_tuple(&m1);
//     s_vec_norm(&w_normal);
//     normal->x = w_normal.x;
//     normal->y = w_normal.y;
//     normal->z = w_normal.z;
//     normal->w =  w_normal.w;
//     return ;
// }

// // fahd
void s_normal_at(t_tuple *normal, t_sphere *s, t_tuple *point)
{
    copy_matrix_contant(s->inv);
    t_matrix m = s_matrix_tuple(s->inv, point);
    t_tuple obj_point = s_matrix_to_tuple(&m);
    t_tuple obj_normal = (t_tuple){0,0,0,0};
    s_sub_t(&obj_normal, obj_point, *(s->origin));
    
    // ONLY apply bump mapping for TYPE 2 (bump maps, not checkerboard)
    if (s->material->has_bump_map && s->material->bump_map && 
        s->material->bump_map->type == 2) {
        
        ld u, v;         
        sphere_uv(point, s, &u, &v);
        
        // Increase UV scale to see more bump detail
        u *= 8.0;
        v *= 8.0;
        
        // Sample bump map height
        ld height = sample_height(s->material->bump_map, u, v);
        
        // Calculate bump normal using finite differences
        ld eps = 0.01;
        ld height_u = sample_height(s->material->bump_map, u + eps, v);
        ld height_v = sample_height(s->material->bump_map, u, v + eps);
        
        // Create bump perturbation
        t_tuple bump_normal = {
            (height - height_u) * s->material->bump_strength * 5.0,
            (height - height_v) * s->material->bump_strength * 5.0,
            1.0, 0
        };
        s_vec_norm(&bump_normal);
        
        // Combine with geometric normal
        obj_normal.x += bump_normal.x;
        obj_normal.y += bump_normal.y;
        obj_normal.z += bump_normal.z;
    }

    t_matrix inv_transpose = s_matrix_transpos(s->inv);
    t_matrix m1 = s_matrix_tuple(&inv_transpose, &obj_normal);
    t_tuple w_normal = s_matrix_to_tuple(&m1);
    s_vec_norm(&w_normal);
    
    normal->x = w_normal.x;
    normal->y = w_normal.y;
    normal->z = w_normal.z;
    normal->w = w_normal.w;
}