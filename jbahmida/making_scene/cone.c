#include "../../MiniRt.h"


void cone(t_cone **cone, t_matrix *trans, bool flag)
{
    if (!cone)
        return;
    static int id = 0;
    if (flag)
    {
        (*cone)->transform = trans;
        (*cone)->inv = matrix_inverte((*cone)->transform);
        return;
    }
    (*cone) = alloc(sizeof(t_cone), false);
    (*cone)->transform = identity_matrix(4, 4);
    (*cone)->inv = identity_matrix(4, 4);
    // (*cone)->origin = tuple_constractor(0, 0, 0, 1);  
    // (*cone)->minimum = -INFINITY;                    
    // (*cone)->maximum = INFINITY;                    
    (*cone)->id = id;
    (*cone)->material = material();
    puts("--------------------------cone ------------------------------\n");

    // (*cone)->closed = false;            
    id++;
}
t_matrix *rotation_from_to(t_tuple *from, t_tuple *to)
{
    // Rotate from 'from' vector to 'to' vector
    t_tuple v = s_cross_product(from, to);
    ld s = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    ld c = dot_product(from, to);
    
    if (s < 0.0001) {
        // Vectors are parallel
        if (c > 0)
            return identity_matrix(4, 4);
        else
            return rotation_x(M_PI);  // 180° flip
    }
    
    // Skew-symmetric matrix for cross product
    ld vx[9] = {
        0,    -v.z,  v.y,
        v.z,   0,   -v.x,
        -v.y,  v.x,  0
    };
    
    // Rodrigues' formula: R = I + [v]× + [v]×² * (1-c)/s²
    t_matrix *result = identity_matrix(4, 4);
    
    ld factor = (1 - c) / (s * s);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result->matrix[i][j] += vx[i*3 + j];
            for (int k = 0; k < 3; k++) {
                result->matrix[i][j] += vx[i*3 + k] * vx[k*3 + j] * factor;
            }
        }
    }
    
    return result;
}

t_matrix *create_cone_transform(t_tuple *position, t_tuple *axis, 
                                ld radius, ld height)
{
    t_tuple normalized_axis = *axis;
    s_vec_norm(&normalized_axis);
    
    t_tuple default_y = {0, 1, 0, 0};
    t_matrix *rotation = rotation_from_to(&default_y, &normalized_axis);
    t_matrix *scale = scaling(radius, height / 2.0, radius);
    t_matrix *trans = translation(position->x, position->y, position->z);
    
    t_matrix *temp = matrix_multi(trans, rotation);
    t_matrix *result = matrix_multi(temp, scale);
    
    copy_matrix_contant(result);
    return result;
}

// t_matrix *create_cone_transform(t_tuple *position, t_tuple *axis, 
//                                 ld radius, ld height)
// {
//     // Normalize the axis vector
//     t_tuple normalized_axis = *axis;
//     s_vec_norm(&normalized_axis);
    
//     // Default Y-axis (cone's natural orientation)
//     t_tuple default_y = {0, 1, 0, 0};
//     t_matrix *rotation;
//     ld dot = dot_product(&default_y, &normalized_axis);
    
//     // Check if axis is already aligned with Y
//     if (fabsf(dot - 1.0) < 0.001)
//         rotation = identity_matrix(4, 4);
//     // Check if axis is opposite to Y
//     else if (fabsf(dot + 1.0) < 0.001)
//         rotation = rotation_x(M_PI);
//     else
//     {
//         // Handle specific axis cases
//         if (fabsf(normalized_axis.x - 1.0) < 0.001)  // X-axis (1,0,0)
//             rotation = rotation_z(-M_PI / 2);
//         else if (fabsf(normalized_axis.x + 1.0) < 0.001)  // -X-axis
//             rotation = rotation_z(M_PI / 2);
//         else if (fabsf(normalized_axis.z - 1.0) < 0.001)  // Z-axis (0,0,1)
//             rotation = rotation_x(M_PI / 2);
//         else if (fabsf(normalized_axis.z + 1.0) < 0.001)  // -Z-axis
//             rotation = rotation_x(-M_PI / 2);
//         else
//             rotation = identity_matrix(4, 4);  // TODO: general case with arbitrary axis
//     }

//     // For a cone, we typically want:
//     // - radius controls the base radius
//     // - height controls the cone height
//     // The cone's equation is: x² + z² = (y/height * radius)²
//     // So we scale by radius in x,z and by height/2 in y (to center it)
//     t_matrix *scale = scaling(radius, height / 2.0, radius);
    
//     // Translate to the desired position
//     t_matrix *trans = translation(position->x, position->y, position->z);
    
//     // Combine: Translation * Rotation * Scale
//     t_matrix *temp = matrix_multi(trans, rotation);
//     t_matrix *result = matrix_multi(temp, scale);
    
//     // Copy to stack matrix for use
//     copy_matrix_contant(result);
    
//     return result;
// }

bool check_cap_cone(t_stack_ray *ray, ld t, ld y)
{
    ld x = ray->point.x + t * ray->vect.x;
    ld z = ray->point.z + t * ray->vect.z;
    return (powf(x, 2) + powf(z, 2)) <= fabsf(y);
}

void intersect_caps_cone(t_cone *cone, t_stack_ray *ray, t_stack_intersections *xs)
{
    // puts("--------------------------intersect cne ------------------------------\n");
    if (!cone->closed || ldbl_cmp(ray->vect.y, 0.0))
        return;
    
    ld t;
    
    t = (cone->minimum - ray->point.y) / ray->vect.y;
    if (check_cap_cone(ray, t, cone->minimum))
    {
        xs->inters_list[xs->count].inters_value = t;
        xs->inters_list[xs->count].sphere = NULL;
        xs->inters_list[xs->count].plane = NULL;
        xs->inters_list[xs->count].cylinder = NULL;
        xs->inters_list[xs->count].cone = cone;
        xs->inters_list[xs->count].existence = true;
        xs->inters_list[xs->count].type = T_CONE;
        xs->count++;
    }
    
    t = (cone->maximum - ray->point.y) / ray->vect.y;
    if (check_cap_cone(ray, t, cone->maximum))
    {
        xs->inters_list[xs->count].inters_value = t;
        xs->inters_list[xs->count].sphere = NULL;
        xs->inters_list[xs->count].plane = NULL;
        xs->inters_list[xs->count].cylinder = NULL;
        xs->inters_list[xs->count].cone = cone;
        xs->inters_list[xs->count].existence = true;
        xs->inters_list[xs->count].type = T_CONE;
        xs->count++;
    }
}

void cone_intersct(t_stack_intersections *xs, t_cone *cone, t_stack_ray *ray)
{
    // CRITICAL: Transform ray to cone's object space first!
    t_stack_ray local_ray = s_transform(ray, cone->inv);
    
    ld a = powf(local_ray.vect.x, 2) - powf(local_ray.vect.y, 2) + powf(local_ray.vect.z, 2);
    ld b = 2 * local_ray.point.x * local_ray.vect.x - 
           2 * local_ray.point.y * local_ray.vect.y + 
           2 * local_ray.point.z * local_ray.vect.z;
    ld c = powf(local_ray.point.x, 2) - powf(local_ray.point.y, 2) + powf(local_ray.point.z, 2);
    
    xs->count = 0;
    xs->existence = false;
    
    if (ldbl_cmp(a, 0.0))
    {
        if (!ldbl_cmp(b, 0.0))
        {
            ld t = -c / (2 * b);
            ld y = local_ray.point.y + t * local_ray.vect.y;
            
            if (y > cone->minimum && y < cone->maximum)
            {
                xs->inters_list[xs->count].inters_value = t;
                xs->inters_list[xs->count].sphere = NULL;
                xs->inters_list[xs->count].cylinder = NULL;
                xs->inters_list[xs->count].plane = NULL;
                xs->inters_list[xs->count].cone = cone;
                xs->inters_list[xs->count].existence = true;
                xs->inters_list[xs->count].type = T_CONE;
                xs->count++;
            }
        }
    }
    else
    {
        ld delta = powf(b, 2) - 4 * a * c;
        
        if (delta >= 0)
        {
            ld t0 = ((-b) - sqrtl(delta)) / (2 * a);
            ld t1 = ((-b) + sqrtl(delta)) / (2 * a);
            
            if (t0 > t1)
            {
                ld tmp = t0;
                t0 = t1;
                t1 = tmp;
            }
            
            ld y0 = local_ray.point.y + t0 * local_ray.vect.y;
            if (y0 > cone->minimum && y0 < cone->maximum)
            {
                xs->inters_list[xs->count].inters_value = t0;
                xs->inters_list[xs->count].sphere = NULL;
                xs->inters_list[xs->count].cylinder = NULL;
                xs->inters_list[xs->count].plane = NULL;
                xs->inters_list[xs->count].cone = cone;
                xs->inters_list[xs->count].existence = true;
                xs->inters_list[xs->count].type = T_CONE;
                xs->count++;
            }
            
            ld y1 = local_ray.point.y + t1 * local_ray.vect.y;
            if (y1 > cone->minimum && y1 < cone->maximum)
            {
                xs->inters_list[xs->count].inters_value = t1;
                xs->inters_list[xs->count].sphere = NULL;
                xs->inters_list[xs->count].cylinder = NULL;
                xs->inters_list[xs->count].plane = NULL;
                xs->inters_list[xs->count].cone = cone;
                xs->inters_list[xs->count].existence = true;
                xs->inters_list[xs->count].type = T_CONE;
                xs->count++;
            }
        }
    }
    
    intersect_caps_cone(cone, &local_ray, xs);  // Pass local_ray here too!
    
    xs->existence = xs->count > 0;
    
    if (!xs->existence)
        xs->world_inters_list = NULL;
}

void s_cone_normal(t_tuple *norm, t_cone *cone, t_tuple *point)
{
    // Transform point to object space
    t_matrix point_matrix = s_matrix_tuple(cone->inv, point);
    t_tuple local_point = s_matrix_to_tuple(&point_matrix);
    
    ld dist = powf(local_point.x, 2) + powf(local_point.z, 2);
    
    t_tuple local_normal;
    
    if (cone->closed && dist < fabsf(cone->maximum) && 
        local_point.y >= cone->maximum - FLT_EPSILON)
    {
        local_normal = (t_tuple){0, 1, 0, 0};
    }
    else if (cone->closed && dist < fabsf(cone->minimum) && 
            local_point.y <= cone->minimum + FLT_EPSILON)
    {
        local_normal = (t_tuple){0, -1, 0, 0};
    }
    else
    {
        ld y = sqrtl(powf(local_point.x, 2) + powf(local_point.z, 2));
        if (local_point.y > 0)
            y = -y;
        
        local_normal.x = local_point.x;
        local_normal.y = y;
        local_normal.z = local_point.z;
        local_normal.w = 0;
    }
    
    // Transform normal to world space
    t_matrix transpose = s_matrix_transpos(cone->inv);
    t_matrix world_normal = s_matrix_tuple(&transpose, &local_normal);
    *norm = s_matrix_to_tuple(&world_normal);
    norm->w = 0;
    s_vec_norm(norm);
}



// void s_cone_normal(t_tuple *norm, t_cone *cone, t_tuple *point)
// {
//     // Transform point to object space
//     t_matrix point_matrix = s_matrix_tuple(cone->inv, point);
//     t_tuple local_point = s_matrix_to_tuple(&point_matrix);

//     ld dist = powf(local_point.x, 2) + powf(local_point.z, 2);

//     t_tuple local_normal;

//     if (cone->closed && dist < fabsf(cone->maximum) && 
//         local_point.y >= cone->maximum - FLT_EPSILON)
//     {
//         local_normal = (t_tuple){0, 1, 0, 0};
//     }
//     else if (cone->closed && dist < fabsf(cone->minimum) && 
//             local_point.y <= cone->minimum + FLT_EPSILON)
//     {
//         local_normal = (t_tuple){0, -1, 0, 0};
//     }
//     else
//     {
//         ld y = sqrtl(powf(local_point.x, 2) + powf(local_point.z, 2));
//         if (local_point.y > 0)
//             y = -y;

//         local_normal.x = local_point.x;
//         local_normal.y = y;
//         local_normal.z = local_point.z;
//         local_normal.w = 0;
//     }

//     // --- Bump mapping for checkerboard (type 1) ---
//     if (cone->material->has_bump_map && cone->material->bump_map &&
//         cone->material->bump_map->type == 1)
//     {
//         ld u, v;
//         cone_uv(point, cone, &u, &v);

//         ld scale = cone->material->bump_map->scale;
//         if (scale < 0.01) scale = 1.0;
//         ld freq = 1.0 / scale;

//         int iu = (int)(u * freq);
//         int iv = (int)(v * freq);
//         ld bump_height = ((iu + iv) % 2 == 0) ? 0.0 : 1.0;

//         ld eps = 0.01;
//         int iu_next = (int)((u + eps) * freq);
//         int iv_next = (int)((v + eps) * freq);

//         ld height_u = ((iu_next + iv) % 2 == 0) ? 0.0 : 1.0;
//         ld height_v = ((iu + iv_next) % 2 == 0) ? 0.0 : 1.0;

//         ld du = (height_u - bump_height) * cone->material->bump_strength * 2.0;
//         ld dv = (height_v - bump_height) * cone->material->bump_strength * 2.0;

//         local_normal.x += du;
//         local_normal.z += dv;
//     }

//     // --- Bump mapping for image (type 2) ---
//     if (cone->material->has_bump_map && cone->material->bump_map &&
//         cone->material->bump_map->type == 2 && cone->material->bump_map->image)
//     {
//         ld u, v;
//         cone_uv(point, cone, &u, &v);

//         ld height = sample_height(cone->material->bump_map, u, v);
//         ld eps = 0.001;
//         ld height_u = sample_height(cone->material->bump_map, u + eps, v);
//         ld height_v = sample_height(cone->material->bump_map, u, v + eps);

//         ld du = (height_u - height) * cone->material->bump_strength * 10.0;
//         ld dv = (height_v - height) * cone->material->bump_strength * 10.0;

//         local_normal.x += du;
//         local_normal.z += dv;
//     }

//     // Transform normal to world space
//     t_matrix transpose = s_matrix_transpos(cone->inv);
//     t_matrix world_normal = s_matrix_tuple(&transpose, &local_normal);
//     *norm = s_matrix_to_tuple(&world_normal);
//     norm->w = 0;
//     s_vec_norm(norm);
// }
