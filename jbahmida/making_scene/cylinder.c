#include "../../MiniRt.h"

void cylinder(t_cylinder **cylinder, t_matrix *trans, bool flag)
{
    if (!cylinder)
        return;
    static int id = 0;
    if (flag)
    {
        (*cylinder)->transform = trans;
        (*cylinder)->inv = matrix_inverte((*cylinder)->transform);
        return;
    }
    (*cylinder) = alloc(sizeof(t_cylinder), false);
    (*cylinder)->transform = identity_matrix(4, 4);
    (*cylinder)->inv = identity_matrix(4, 4);
    (*cylinder)->minimum = -INFINITY;
    (*cylinder)->maximum = INFINITY; 
    (*cylinder)->id = id;
    (*cylinder)->material = material();
    (*cylinder)->closed = false;
    id++;
}

// t_matrix *create_cylinder_transform(t_tuple *position, t_tuple *axis, 
//                                     ld radius, ld height)
// {
//     t_tuple normalized_axis = *axis;
//     s_vec_norm(&normalized_axis);
    
//     t_tuple default_y = {0, 1, 0, 0};
//     t_matrix *rotation;
//     ld dot = dot_product(&default_y, &normalized_axis);
    
//     if (fabsf(dot - 1.0) < 0.001)
//         rotation = identity_matrix(4, 4);
//     else if (fabsf(dot + 1.0) < 0.001)
//         rotation = rotation_x(M_PI);
//     else
//     {
//         if (fabsf(normalized_axis.x - 1.0) < 0.001)  // X-axis (1,0,0)
//             rotation = rotation_z(-M_PI / 2);
//         else if (fabsf(normalized_axis.x + 1.0) < 0.001)  // -X-axis
//             rotation = rotation_z(M_PI / 2);
//         else if (fabsf(normalized_axis.z - 1.0) < 0.001)  // Z-axis (0,0,1)
//             rotation = rotation_x(M_PI / 2);
//         else if (fabsf(normalized_axis.z + 1.0) < 0.001)  // -Z-axis
//             rotation = rotation_x(-M_PI / 2);
//         else
//             rotation = identity_matrix(4, 4);  // TODO: general case
//     }

//     t_matrix *scale = scaling(radius, height / 2.0, radius);
//     t_matrix *trans = translation(position->x, position->y, position->z);
//     t_matrix *temp = matrix_multi(trans, rotation);
//     t_matrix *result = matrix_multi(temp, scale);
//     copy_matrix_contant(result);
//     return result;
// }
void skew_symmetric(t_tuple *axis, t_matrix *result) {
    result->matrix[0][0] = 0;       result->matrix[0][1] = -axis->z;  result->matrix[0][2] = axis->y;  result->matrix[0][3] = 0;
    result->matrix[1][0] = axis->z;  result->matrix[1][1] = 0;       result->matrix[1][2] = -axis->x; result->matrix[1][3] = 0;
    result->matrix[2][0] = -axis->y; result->matrix[2][1] = axis->x;  result->matrix[2][2] = 0;       result->matrix[2][3] = 0;
    result->matrix[3][0] = 0;       result->matrix[3][1] = 0;       result->matrix[3][2] = 0;       result->matrix[3][3] = 1;
}

// Function to compute the rotation matrix around an arbitrary axis by an angle
t_matrix *rotation_axis_angle(t_tuple *axis, ld angle) {
    t_matrix *rot = identity_matrix(4, 4);

    t_tuple norm_axis = *axis;
    s_vec_norm(&norm_axis);
    t_matrix *skew = matrix_constractor(4,4,(ld[16]){0}, 16);
    skew_symmetric(&norm_axis, skew);
    
    // Compute cos and sin of the angle
    ld cos_theta = cos(angle);
    ld sin_theta = sin(angle);
    
    // Update the rotation matrix using Rodrigues' formula
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // The rotation matrix is the sum of the identity, outer product, and skew-symmetric part
            rot->matrix[i][j] = 
                (i == j ? 1 : 0) * cos_theta  // Identity part
                + (1 - cos_theta) * (norm_axis.x * norm_axis.x * (i == 0) + norm_axis.y * norm_axis.y * (i == 1) + norm_axis.z * norm_axis.z * (i == 2))  // Outer product part
                + skew->matrix[i][j] * sin_theta;  // Skew-symmetric part
        }
    }
    
    return rot;
}
t_matrix *create_cylinder_transform(t_tuple *position, t_tuple *axis, 
                                    ld radius, ld height)
{
    t_tuple normalized_axis = *axis;
    s_vec_norm(&normalized_axis);
    
    t_tuple default_y = {0, 1, 0, 0};
    ld dot = dot_product(&default_y, &normalized_axis);
    dot = fmax(fmin(dot, 1.0), -1.0); // Clamp to avoid NaN from acos

    t_matrix *rotation = NULL;

    if (fabs(dot - 1.0) < 1e-3)
        rotation = identity_matrix(4, 4);
    else if (fabs(dot + 1.0) < 1e-3)
        rotation = rotation_x(M_PI);
    else
    {
        t_tuple rot_axis = *cross_product(&default_y, &normalized_axis);
        s_vec_norm(&rot_axis);
        ld angle = acos(dot);
        rotation = rotation_axis_angle(&rot_axis, angle);
    }

    t_matrix *scale = scaling(radius, height / 2.0, radius);
    t_matrix *trans = translation(position->x, position->y, position->z);

    t_matrix *temp = matrix_multi(trans, rotation);
    t_matrix *result = matrix_multi(temp, scale);
    copy_matrix_contant(result);
    return result;
}

bool check_cap_cylinder(t_stack_ray *ray, ld t)
{
    ld x = ray->point.x + t * ray->vect.x;
    ld z = ray->point.z + t * ray->vect.z;
    return (powf(x, 2) + powf(z, 2)) <= 1;
}

void intersect_caps_cylinder(t_cylinder *cylinder, t_stack_ray *ray, t_stack_intersections *xs)
{
    if (!cylinder->closed || ldbl_cmp(ray->vect.y, 0.0))
        return;
    
    ld t;
    
    t = (cylinder->minimum - ray->point.y) / ray->vect.y;
    if (check_cap_cylinder(ray, t))
    {
        xs->inters_list[xs->count].inters_value = t;
        xs->inters_list[xs->count].sphere = NULL;
        xs->inters_list[xs->count].plane = NULL;
        xs->inters_list[xs->count].cylinder = cylinder;
        xs->inters_list[xs->count].cone = NULL;
        xs->inters_list[xs->count].existence = true;
        xs->inters_list[xs->count].type = T_CYLINDRE;
        xs->count++;
    }
    
    t = (cylinder->maximum - ray->point.y) / ray->vect.y;
    if (check_cap_cylinder(ray, t))
    {
        xs->inters_list[xs->count].inters_value = t;
        xs->inters_list[xs->count].sphere = NULL;
        xs->inters_list[xs->count].plane = NULL;
        xs->inters_list[xs->count].cylinder = cylinder;
        xs->inters_list[xs->count].cone = NULL;
        xs->inters_list[xs->count].existence = true;
        xs->inters_list[xs->count].type = T_CYLINDRE;
        xs->count++;
    }
}


// b ← 2 * ray.origin.x * ray.direction.x +
// 2 * ray.origin.z * ray.direction.z
// c ← ray.origin.x² + ray.origin.z² - 1
void cylinder_intersct(t_stack_intersections *xs, t_cylinder *cylinder, t_stack_ray *ray)
{
    t_stack_ray local_ray = s_transform(ray, cylinder->inv);
    // t_stack_ray local_ray = *ray;
    
    ld a = powf(local_ray.vect.x, 2) + powf(local_ray.vect.z, 2);
    
    xs->count = 0;
    xs->existence = false;
    if (!ldbl_cmp(a, 0.0))
    {
        ld b = (2 * local_ray.point.x * local_ray.vect.x) + (2 * local_ray.point.z * local_ray.vect.z);
        ld c = powf(local_ray.point.x, 2) + powf(local_ray.point.z, 2) - 1;
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
            if (y0 > cylinder->minimum && y0 < cylinder->maximum)
            {
                xs->inters_list[xs->count].inters_value = t0;
                xs->inters_list[xs->count].sphere = NULL;
                xs->inters_list[xs->count].plane = NULL;
                xs->inters_list[xs->count].cylinder = cylinder;
                xs->inters_list[xs->count].cone = NULL;
                xs->inters_list[xs->count].existence = true;
                xs->inters_list[xs->count].type = T_CYLINDRE;
                xs->count++;
            }
            
            ld y1 = local_ray.point.y + t1 * local_ray.vect.y;
            if (y1 > cylinder->minimum && y1 < cylinder->maximum)
            {
                xs->inters_list[xs->count].inters_value = t1;
                xs->inters_list[xs->count].sphere = NULL;
                xs->inters_list[xs->count].plane = NULL;
                xs->inters_list[xs->count].cylinder = cylinder;
                xs->inters_list[xs->count].cone = NULL;
                xs->inters_list[xs->count].existence = true;
                xs->inters_list[xs->count].type = T_CYLINDRE;
                xs->count++;
            }
        }
    }
    
    intersect_caps_cylinder(cylinder, &local_ray, xs);
    xs->existence = xs->count > 0;
    if (!xs->existence)
        xs->world_inters_list = NULL;
}

// void s_cylinder_normal(t_tuple *norm, t_cylinder *cylinder, t_tuple *point)
// {
//     ld dist = powf(point->x, 2) + powf(point->z, 2);
    
//     if (dist < 1 && point->y >= cylinder->maximum - FLT_EPSILON)
//     {
//         // Normal for top cap
//         norm->x = 0;
//         norm->y = 1;
//         norm->z = 0;
//         norm->w = 0;
//     }
//     else if (dist < 1 && point->y <= cylinder->minimum + FLT_EPSILON)
//     {
//         norm->x = 0;
//         norm->y = -1;
//         norm->z = 0;
//         norm->w = 0;
//     }
//     else
//     {
//         norm->x = point->x;
//         norm->y = 0;
//         norm->z = point->z;
//         norm->w = 0;
//     }
// }

// cylinder.c
// void s_cylinder_normal(t_tuple *norm, t_cylinder *cylinder, t_tuple *point)
// {
//     // Transform point to object space
//     t_matrix point_matrix = s_matrix_tuple(cylinder->inv, point);
//     t_tuple local_point = s_matrix_to_tuple(&point_matrix);
    
//     ld dist = powf(local_point.x, 2) + powf(local_point.z, 2);
    
//     t_tuple local_normal;
    
//     if (cylinder->closed && dist < 1 && local_point.y >= cylinder->maximum - FLT_EPSILON)
//     {
//         local_normal = (t_tuple){0, 1, 0, 0};
//     }
//     else if (cylinder->closed && dist < 1 && local_point.y <= cylinder->minimum + FLT_EPSILON)
//     {
//         local_normal = (t_tuple){0, -1, 0, 0};
//     }
//     else
//     {
//         // Side normal
//         local_normal = (t_tuple){local_point.x, 0, local_point.z, 0};
//     }
    
//     // ✅ Transform normal back to world space
//     t_matrix transpose = s_matrix_transpos(cylinder->inv);
//     t_matrix world_normal_matrix = s_matrix_tuple(&transpose, &local_normal);
//     *norm = s_matrix_to_tuple(&world_normal_matrix);
//     norm->w = 0;
//     s_vec_norm(norm);
// }


void s_cylinder_normal(t_tuple *norm, t_cylinder *cylinder, t_tuple *point)
{
    // Transform point to object space
    t_matrix point_matrix = s_matrix_tuple(cylinder->inv, point);
    t_tuple local_point = s_matrix_to_tuple(&point_matrix);

    ld dist = powf(local_point.x, 2) + powf(local_point.z, 2);
    t_tuple local_normal;

    if (cylinder->closed && dist < 1 && local_point.y >= cylinder->maximum - FLT_EPSILON)
        local_normal = (t_tuple){0, 1, 0, 0};
    else if (cylinder->closed && dist < 1 && local_point.y <= cylinder->minimum + FLT_EPSILON)
        local_normal = (t_tuple){0, -1, 0, 0};
    else
        local_normal = (t_tuple){local_point.x, 0, local_point.z, 0};

    // ✅ FIXED BUMP MAPPING for checkerboard
    if (cylinder->material->has_bump_map && cylinder->material->bump_map)
    {
        ld u, v;
        cylinder_uv(point, cylinder, &u, &v);

        if (cylinder->material->bump_map->type == 1)
        {
            // ✅ FIX: Calculate checkerboard BEFORE wrapping
            ld scale = cylinder->material->bump_map->scale;
            if (scale < 0.01) scale = 1.0;  // Prevent division by zero
            
            ld freq = 1.0 / scale;
            
            // ✅ CRITICAL: Multiply AFTER wrapping, not before
            int iu = (int)(u * freq);
            int iv = (int)(v * freq);
            
            ld bump_height = ((iu + iv) % 2 == 0) ? 0.0 : 1.0;

            // Calculate gradients for bump
            ld eps = 0.01;
            int iu_next = (int)((u + eps) * freq);
            int iv_next = (int)((v + eps) * freq);
            
            ld height_u = ((iu_next + iv) % 2 == 0) ? 0.0 : 1.0;
            ld height_v = ((iu + iv_next) % 2 == 0) ? 0.0 : 1.0;

            ld du = (height_u - bump_height) * cylinder->material->bump_strength * 2.0;
            ld dv = (height_v - bump_height) * cylinder->material->bump_strength * 2.0;

            local_normal.x += du;
            local_normal.z += dv;
            printf("u=%.3f v=%.3f iu=%d iv=%d pattern=%d\n", u, v, iu, iv, (iu + iv) % 2);

        }
       

    }
     if (cylinder->material->has_bump_map && cylinder->material->bump_map &&
        cylinder->material->bump_map->type == 2 && cylinder->material->bump_map->image)
        {
            ld u, v;
            cylinder_uv(point, cylinder, &u, &v);

            // Image bump mapping
            ld height = sample_height(cylinder->material->bump_map, u, v);
            ld eps = 0.001;
            ld height_u = sample_height(cylinder->material->bump_map, u + eps, v);
            ld height_v = sample_height(cylinder->material->bump_map, u, v + eps);

            ld du = (height_u - height) * cylinder->material->bump_strength * 10.0;
            ld dv = (height_v - height) * cylinder->material->bump_strength * 10.0;

            local_normal.x += du;
            local_normal.z += dv;
        }
    // Transform normal back to world space
    t_matrix transpose = s_matrix_transpos(cylinder->inv);
    t_matrix world_normal_matrix = s_matrix_tuple(&transpose, &local_normal);
    *norm = s_matrix_to_tuple(&world_normal_matrix);
    norm->w = 0;
    s_vec_norm(norm);
}