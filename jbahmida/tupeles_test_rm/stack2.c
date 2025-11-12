#include "../../MiniRt.h"

extern int g_fd;
// add_t
// sub_t
// scalar_mult
// normal_at
//matrix_transpos
//vec_norm

void s_add_t(t_tuple *res, t_tuple t1, t_tuple t2)
{
    res->x = t1.x + t2.x;
    res->y = t1.y + t2.y;
    res->z = t1.z + t2.z;
    res->w = t1.w + t2.w;
    return ;
}

void s_sub_t(t_tuple *res, t_tuple t1, t_tuple t2)
{
    res->x = t1.x - t2.x;
    res->y = t1.y - t2.y;
    res->z = t1.z - t2.z;
    res->w = 0;
    return ;
}

void s_scalar_mult(t_tuple *res, t_tuple t, ld scalar)
{
    res->x = t.x * scalar;
    res->y = t.y * scalar;
    res->z = t.z * scalar;
    res->w = t.w;
    return ;
}


t_matrix s_matrix_transpos(t_matrix *m)
{
    t_matrix *tmp;
    t_matrix tran;
    // ld *va_list;
    int i = 0;
    int j;
    tmp = m;
    while (i < m->row)
    {
        j = 0;
        while (j < m->col)
        {
            tran.stack_matrix[j][i] = tmp->matrix[i][j];
            j++;
        }
        i++;
    }

    return tran;
}

void s_vec_norm(t_tuple *t)
{
    if (!magn(t))
        return ;
    ld mgn = magn(t);
    t->x = t->x / mgn;
    t->y = t->y / mgn;
    t->z = t->z / mgn;
    t->w = 0;
}

// void copy_matrix_contant(t_matrix *m)
// {
//     if(!m)
//         return ;
//     for(int i = 0; i < m->row; i++)
//         for(int j = 0; j < m->col; j++)
//             m->stack_matrix[i][j] = m->matrix[i][j];
//     return ;
// }





t_tuple s_neg_t(t_tuple t)
{
    t_tuple local;

    local = (t_tuple){0,0,0,0};
    local.x = -(t.x);
    local.y = -(t.y);
    local.z = -(t.z);
    local.w = 0;
    return local;
}
void s_neg_t1(t_tuple *t)
{
    t->x = -(t->x);
    t->y = -(t->y);
    t->z = -(t->z);
    t->w = 0;
}

t_color s_hadamard(t_color *c1, t_color *c2)
{
    t_color local;

    local = (t_color){0,0,0};
    local.r = c1->r * c2->r;
    local.g = c1->g * c2->g;
    local.b = c1->b * c2->b;
    return local;
}

t_color s_color_scalar(t_color *c, ld s)
{
    t_color local;
    local = (t_color){0,0,0};

    local.r = c->r * s;
    local.g = c->g * s;
    local.b = c->b * s;
    return local;
}

t_color s_color_add(t_color *c1, t_color *c2)
{
    t_color local;
    local = (t_color){0,0,0};

    local.r = c1->r + c2->r;
    local.g = c1->g + c2->g;
    local.b = c1->b + c2->b;
    return local;
}

t_tuple s_reflect(t_tuple in, t_tuple normal)
{
    t_tuple local;
    t_tuple local1;
    local = (t_tuple){0,0,0,0};
    local1 = (t_tuple){0,0,0,0};
    ld t = (2 * dot_product(&in, &normal));
    s_scalar_mult(&local1, normal, t);
    s_sub_t(&local, in, local1);
    return local;
}

static inline ld tone_map(ld x)
{
    return (x / (1.0 + x));
}

static inline ld gamma_correct(ld x, ld gamma)
{
    return pow(x, 1.0 / gamma);
}

// t_color s_lighting(t_material material, t_light light, 
//     t_tuple position, t_tuple eyev, t_tuple normalv)
// {

//     t_color ambient  = (t_color){0, 0, 0};
//     t_color diffuse  = (t_color){0, 0, 0};
//     t_color specular = (t_color){0, 0, 0};
//     t_color effective_color = s_hadamard((material.color), light.intensity); // Add &
//     t_tuple lightv;
//     lightv = (t_tuple){0,0,0,0};
//     s_sub_t(&lightv, *(light.position), position);
//     s_vec_norm(&lightv);
//     ambient = s_color_scalar(&effective_color, material.ambient);
    
//     ld light_dot_normal = dot_product(&lightv, &normalv);
//     if (light_dot_normal >= 0)
//     {
//         diffuse = s_color_scalar(&effective_color,
//             material.diffuse * light_dot_normal);

//         lightv = s_neg_t(lightv);
//         t_tuple reflectv = s_reflect(lightv, normalv);
//         s_vec_norm(&reflectv);
//         s_vec_norm(&eyev);
//         ld reflect_dot_eye = dot_product(&reflectv, &eyev);
//         if (reflect_dot_eye > 0)
//         {
//             ld factor = pow(reflect_dot_eye, material.shininess);
//             specular = s_color_scalar(light.intensity,
//                                     (material.specular * factor));
//         }
//     }

//     t_color base_color = s_color_add(&ambient, &diffuse);
//     t_color result = s_color_add(&base_color, &specular);
//     // result.r = gamma_correct(tone_map(result.r), 1);
//     // result.g = gamma_correct(tone_map(result.g), 1);
//     // result.b = gamma_correct(tone_map(result.b), 1);
//     result.r = (result.r > 1.0) ? 1.0 : (result.r < 0.0) ? 0.0 : result.r;
//     result.g = (result.g > 1.0) ? 1.0 : (result.g < 0.0) ? 0.0 : result.g;
//     result.b = (result.b > 1.0) ? 1.0 : (result.b < 0.0) ? 0.0 : result.b;
//     return result;
// }

// t_color s_lighting(t_material material, t_light light, t_tuple position, t_tuple eyev, t_tuple normalv, bool in_shadow)
// {
//     t_color ambient = {0, 0, 0};
//     t_color diffuse = {0, 0, 0};
//     t_color specular = {0, 0, 0};
//     t_color effective_color = s_hadamard(material.color, light.intensity);
//     t_tuple lightv = {0, 0, 0, 0};
//     t_color base_color = (t_color){0,0,0};
//     t_color result = (t_color){0,0,0};
//     s_sub_t(&lightv, *(light.position), position);
//     s_vec_norm(&lightv);
//     ambient = s_color_scalar(&effective_color, material.ambient);
    
//     if (in_shadow)
//     {
//         ambient.r = (ambient.r > 1.0) ? 1.0 : (ambient.r < 0.0) ? 0.0 : ambient.r;
//         ambient.g = (ambient.g > 1.0) ? 1.0 : (ambient.g < 0.0) ? 0.0 : ambient.g;
//         ambient.b = (ambient.b > 1.0) ? 1.0 : (ambient.b < 0.0) ? 0.0 : ambient.b;
//         return ambient;
//     }   
//     else
//     {
//         ld light_dot_normal = dot_product(&lightv, &normalv);
//         if (light_dot_normal >= 0)
//         {
//             diffuse = s_color_scalar(&effective_color, material.diffuse * light_dot_normal);
//             t_tuple neg_lightv = s_neg_t(lightv);
//             t_tuple reflectv = s_reflect(neg_lightv, normalv);
//             s_vec_norm(&reflectv);
//             s_vec_norm(&eyev);
//             ld reflect_dot_eye = dot_product(&reflectv, &eyev);
//             if (reflect_dot_eye > 0)
//             {
//                 ld factor = pow(reflect_dot_eye, material.shininess);
//                 specular = s_color_scalar(light.intensity, material.specular * factor);
//             }
//         }
//         base_color = s_color_add(&ambient, &diffuse);
//         result = s_color_add(&base_color, &specular);
//         result.r = (result.r > 1.0) ? 1.0 : (result.r < 0.0) ? 0.0 : result.r;
//         result.g = (result.g > 1.0) ? 1.0 : (result.g < 0.0) ? 0.0 : result.g;
//         result.b = (result.b > 1.0) ? 1.0 : (result.b < 0.0) ? 0.0 : result.b;
//     }
//     // result.r = gamma_correct(tone_map(result.r), 1);
//     // result.g = gamma_correct(tone_map(result.g), 1);
//     // result.b = gamma_correct(tone_map(result.b), 1);
//     return (result);
// }


// t_color s_lighting(t_material material, t_light light, t_tuple position, 
//                    t_tuple eyev, t_tuple normalv, bool in_shadow)
// {
//     t_color ambient = {0, 0, 0};
//     t_color diffuse = {0, 0, 0};
//     t_color specular = {0, 0, 0};
//     t_color effective_color = s_hadamard(material.color, light.intensity);
//     t_tuple lightv = {0, 0, 0, 0};
    
//     // Calculate light direction vector
//     s_sub_t(&lightv, *(light.position), position);
//     s_vec_norm(&lightv);
    
//     // Calculate ambient contribution (always present)
//     ambient = s_color_scalar(&effective_color, material.ambient);
    
//     // If in shadow, return only ambient lighting
//     if (in_shadow)
//     {
//         ambient.r = (ambient.r > 1.0) ? 1.0 : (ambient.r < 0.0) ? 0.0 : ambient.r;
//         ambient.g = (ambient.g > 1.0) ? 1.0 : (ambient.g < 0.0) ? 0.0 : ambient.g;
//         ambient.b = (ambient.b > 1.0) ? 1.0 : (ambient.b < 0.0) ? 0.0 : ambient.b;
//         return ambient;
//     }
    
//     // Calculate diffuse and specular contributions
//     ld light_dot_normal = dot_product(&lightv, &normalv);
    
//     if (light_dot_normal >= 0)
//     {
//         // Calculate diffuse contribution
//         diffuse = s_color_scalar(&effective_color, material.diffuse * light_dot_normal);
        
//         // Calculate reflection vector
//         t_tuple neg_lightv = s_neg_t(lightv);
//         t_tuple reflectv = s_reflect(neg_lightv, normalv);
//         s_vec_norm(&reflectv);
//         s_vec_norm(&eyev);
        
//         // Calculate specular contribution
//         ld reflect_dot_eye = dot_product(&reflectv, &eyev);
//         if (reflect_dot_eye > 0)
//         {
//             ld factor = pow(reflect_dot_eye, material.shininess);
//             specular = s_color_scalar(light.intensity, material.specular * factor);
//         }
//     }
    
//     // Combine all lighting components
//     t_color base_color = s_color_add(&ambient, &diffuse);
//     t_color result = s_color_add(&base_color, &specular);
    
//     // Clamp final color to [0, 1] range
//     result.r = (result.r > 1.0) ? 1.0 : (result.r < 0.0) ? 0.0 : result.r;
//     result.g = (result.g > 1.0) ? 1.0 : (result.g < 0.0) ? 0.0 : result.g;
//     result.b = (result.b > 1.0) ? 1.0 : (result.b < 0.0) ? 0.0 : result.b;
    
//     return result;
// }


t_color s_lighting(t_material material, t_light light, t_tuple position, 
                   t_tuple eyev, t_tuple normalv, bool in_shadow)
{
    t_color diffuse = {0, 0, 0};
    t_color specular = {0, 0, 0};
    t_color effective_color = s_hadamard(material.color, light.intensity);
    t_tuple lightv = {0, 0, 0, 0};
    
    // Calculate light direction vector
    s_sub_t(&lightv, *(light.position), position);
    s_vec_norm(&lightv);
    
    // NOTE: Ambient should only be calculated ONCE for the entire scene
    // Not per light! Move ambient calculation outside this function
    // For now, we'll include a small ambient per light and it will accumulate
    
    // If in shadow from this light, return only black (no contribution)
    if (in_shadow)
    {
        return (t_color){0, 0, 0};
    }
    
    // Calculate diffuse and specular contributions
    ld light_dot_normal = dot_product(&lightv, &normalv);
    
    if (light_dot_normal >= 0)
    {
        // Calculate diffuse contribution
        diffuse = s_color_scalar(&effective_color, material.diffuse * light_dot_normal);
        
        // Calculate reflection vector
        t_tuple neg_lightv = s_neg_t(lightv);
        t_tuple reflectv = s_reflect(neg_lightv, normalv);
        s_vec_norm(&reflectv);
        s_vec_norm(&eyev);
        
        // Calculate specular contribution
        ld reflect_dot_eye = dot_product(&reflectv, &eyev);
        if (reflect_dot_eye > 0)
        {
            ld factor = pow(reflect_dot_eye, material.shininess);
            specular = s_color_scalar(light.intensity, material.specular * factor);
        }
    }
    
    // Combine diffuse and specular (ambient added separately)
    t_color result = s_color_add(&diffuse, &specular);
    
    return result;
}

t_color shade_hit_with_ambient(t_world *world, t_comp *comp)
{
    t_material *material = get_material(comp);
    
    if (material == NULL)
        return (t_color){0, 0, 0};
    
    // Calculate ambient ONCE (not per light)
    t_color ambient = s_color_scalar(material->color, material->ambient);
    
    t_color total_lighting = {0, 0, 0};
    
    // Loop through all lights and accumulate lighting
    for (int i = 0; i < world->light_num; i++)
    {
        bool shadowed = is_shadowed(world, comp->over_point, world->lights[i]);
        
        t_color light_contribution = s_lighting(*material, *(world->lights[i]), 
                                                comp->over_point, comp->eyev, 
                                                comp->normalv, shadowed);
        
        total_lighting.r += light_contribution.r;
        total_lighting.g += light_contribution.g;
        total_lighting.b += light_contribution.b;
    }
    
    // Add ambient to total lighting
    t_color final_color = s_color_add(&ambient, &total_lighting);
    
    // Clamp final color to [0, 1]
    final_color.r = (final_color.r > 1.0) ? 1.0 : (final_color.r < 0.0) ? 0.0 : final_color.r;
    final_color.g = (final_color.g > 1.0) ? 1.0 : (final_color.g < 0.0) ? 0.0 : final_color.g;
    final_color.b = (final_color.b > 1.0) ? 1.0 : (final_color.b < 0.0) ? 0.0 : final_color.b;
    
    return final_color;
}