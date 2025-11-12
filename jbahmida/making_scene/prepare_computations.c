#include "../../MiniRt.h"

t_material* get_material(t_comp *comp)
{
    if (comp->obj_type == T_SPHERE)
    {
        t_sphere *sphere = (t_sphere *)comp->obj;
        return sphere->material;
    }
    else if (comp->obj_type == T_CYLINDRE)
    {
        t_cylinder *cylinder = (t_cylinder *)comp->obj;
        return cylinder->material;
    }
    else if (comp->obj_type == T_CONE)
    {
        t_cone *cone = (t_cone *)comp->obj;
        return cone->material;
    }
    else if (comp->obj_type == T_PLAN)
    {
        t_plane *plane = (t_plane *)comp->obj;
        return plane->material;
    }
    return NULL;
}


// t_color shade_hit(t_world *world, t_comp *comp)
// {
//     t_color shade = (t_color){0, 0, 0};
//     bool shadowed;
    
//     shadowed = is_shadowed(world, comp->over_point);
//     t_material *material = get_material(comp);
    
//     if (material == NULL)
//         return shade;
//     shade = s_lighting(*material, *(world->light), comp->over_point, 
//                        comp->eyev, comp->normalv, shadowed);
    
//     return (shade);
// }

// t_color shade_hit(t_world *world, t_comp *comp) ///// jassim 
// {
//     t_color final_color = {0, 0, 0};
//     t_material *material = get_material(comp);
    
//     if (material == NULL)
//         return final_color;
    
//     // Loop through all lights and accumulate lighting
//     for (int i = 0; i < world->light_num; i++)
//     {
//         bool shadowed = is_shadowed(world, comp->over_point, world->lights[i]);
        
//         t_color light_contribution = s_lighting(*material, *(world->lights[i]), 
//                                                 comp->over_point, comp->eyev, 
//                                                 comp->normalv, shadowed);
        
//         // Add this light's contribution to final color
//         final_color.r += light_contribution.r;
//         final_color.g += light_contribution.g;
//         final_color.b += light_contribution.b;
//     }
    
//     // Clamp final color to [0, 1]
//     final_color.r = (final_color.r > 1.0) ? 1.0 : (final_color.r < 0.0) ? 0.0 : final_color.r;
//     final_color.g = (final_color.g > 1.0) ? 1.0 : (final_color.g < 0.0) ? 0.0 : final_color.g;
//     final_color.b = (final_color.b > 1.0) ? 1.0 : (final_color.b < 0.0) ? 0.0 : final_color.b;
    
//     return final_color;
// }


/*---------------------------------------------------------------------------------------------*/ // fahd
// khdama
t_color shade_hit(t_world *world, t_comp *comp) // mee
{
    t_color final_color = {0, 0, 0};
    t_material *material = get_material(comp);
    
    if (material == NULL)
        return final_color;
    
    // START with object's original color
    t_color base_color = *(material->color);
    
    // TYPE 1: Checkerboard COLOR texture - REPLACES object color
    if (material->has_color_texture && material->color_texture && 
        material->color_texture->type == 1) {
        
        ld u, v;
        
        // Get UV coordinates based on object type
        if (comp->obj_type == T_SPHERE) {
            sphere_uv(&comp->point, (t_sphere *)comp->obj, &u, &v);
        } 
        else if (comp->obj_type == T_PLAN) {
            plane_uv(&comp->point, (t_plane *)comp->obj, &u, &v);
        } 
        else if (comp->obj_type == T_CYLINDRE) {
            // printf("CYL UV: u=%.3f v=%.3f\n", u, v);
            cylinder_uv(&comp->point, (t_cylinder *)comp->obj, &u, &v);
        }
        // else if (comp->obj_type == T_CONE) {
        //     cone_uv(&comp->point, (t_cone *)comp->obj, &u, &v);
        // }
        
        // Sample checkerboard texture and REPLACE color
        t_vec3 tex_color = sample_color(material->color_texture, u, v);
        base_color.r = tex_color.x;
        base_color.g = tex_color.y;
        base_color.b = tex_color.z;
    }
    // TYPE 2: Image texture - REPLACES object color
    else if (material->has_bump_map && material->bump_map && 
             material->bump_map->type == 2 && material->bump_map->image) {
        
        ld u, v;
        
        if (comp->obj_type == T_SPHERE) {
            sphere_uv(&comp->point, (t_sphere *)comp->obj, &u, &v);
        } 
        else if (comp->obj_type == T_PLAN) {
            plane_uv(&comp->point, (t_plane *)comp->obj, &u, &v);
        } 
        else if (comp->obj_type == T_CYLINDRE) {
            cylinder_uv(&comp->point, (t_cylinder *)comp->obj, &u, &v);
        }
        // else if (comp->obj_type == T_CONE) {
        //     cone_uv(&comp->point, (t_cone *)comp->obj, &u, &v);
        // }
        
        // Sample texture color
        t_vec3 tex_color = sample_color(material->bump_map, u, v);
        base_color.r = tex_color.x;
        base_color.g = tex_color.y;
        base_color.b = tex_color.z;
    }
    
    // Create temporary material with modified color
    t_material temp_material = *material;
    temp_material.color = &base_color;
    
    // Loop through all lights and accumulate lighting
    for (int i = 0; i < world->light_num; i++) {
        bool shadowed = is_shadowed(world, comp->over_point, world->lights[i]);
        
        t_color light_contribution = s_lighting(temp_material, *(world->lights[i]), 
                                                comp->over_point, comp->eyev, 
                                                comp->normalv, shadowed);
        
        final_color.r += light_contribution.r;
        final_color.g += light_contribution.g;
        final_color.b += light_contribution.b;
    }
    
    // Clamp final color to [0, 1]
    final_color.r = (final_color.r > 1.0) ? 1.0 : (final_color.r < 0.0) ? 0.0 : final_color.r;
    final_color.g = (final_color.g > 1.0) ? 1.0 : (final_color.g < 0.0) ? 0.0 : final_color.g;
    final_color.b = (final_color.b > 1.0) ? 1.0 : (final_color.b < 0.0) ? 0.0 : final_color.b;
    
    return final_color;
}

// khdama
// t_color shade_hit(t_world *world, t_comp *comp)
// {
//     t_color final_color = {0, 0, 0};
//     t_material *material = get_material(comp);
    
//     if (material == NULL)
//         return final_color;
    
//     // START with object's original color
//     t_color base_color = *(material->color);
    
//     // ✅ TYPE 1: Checkerboard COLOR texture - REPLACES object color
//     if (material->has_color_texture && material->color_texture && 
//         material->color_texture->type == 1) {
        
//         ld u, v;
        
//         // Get UV coordinates based on object type
//         if (comp->obj_type == T_SPHERE) {
//             sphere_uv(&comp->point, (t_sphere *)comp->obj, &u, &v);
//         } 
//         else if (comp->obj_type == T_PLAN) {
//             plane_uv(&comp->point, (t_plane *)comp->obj, &u, &v);
//         } 
//         else if (comp->obj_type == T_CYLINDRE) {
//             cylinder_uv(&comp->point, (t_cylinder *)comp->obj, &u, &v);
//         }
//         else if (comp->obj_type == T_CONE) {
//             cone_uv(&comp->point, (t_cone *)comp->obj, &u, &v);
//         }
        
//         // ✅ Sample checkerboard texture and REPLACE color
//         t_vec3 tex_color = sample_color(material->color_texture, u, v);
//         base_color.r = tex_color.x;
//         base_color.g = tex_color.y;
//         base_color.b = tex_color.z;
//     }
//     // TYPE 2: Image texture
//     else if (material->has_bump_map && material->bump_map && 
//              material->bump_map->type == 2 && material->bump_map->image) {
        
//         ld u, v;
        
//         if (comp->obj_type == T_SPHERE) {
//             sphere_uv(&comp->point, (t_sphere *)comp->obj, &u, &v);
//         } 
//         else if (comp->obj_type == T_PLAN) {
//             plane_uv(&comp->point, (t_plane *)comp->obj, &u, &v);
//         } 
//         else if (comp->obj_type == T_CYLINDRE) {
//             cylinder_uv(&comp->point, (t_cylinder *)comp->obj, &u, &v);
//         }
//         else if (comp->obj_type == T_CONE) {
//             cone_uv(&comp->point, (t_cone *)comp->obj, &u, &v);
//         }
        
//         t_vec3 tex_color = sample_color(material->bump_map, u, v);
//         base_color.r = tex_color.x;
//         base_color.g = tex_color.y;
//         base_color.b = tex_color.z;
//     }
    
//     // Create temporary material with modified color
//     t_material temp_material = *material;
//     temp_material.color = &base_color;
    
//     // Loop through all lights and accumulate lighting
//     for (int i = 0; i < world->light_num; i++) {
//         bool shadowed = is_shadowed(world, comp->over_point, world->lights[i]);
        
//         t_color light_contribution = s_lighting(temp_material, *(world->lights[i]), 
//                                                 comp->over_point, comp->eyev, 
//                                                 comp->normalv, shadowed);
        
//         final_color.r += light_contribution.r;
//         final_color.g += light_contribution.g;
//         final_color.b += light_contribution.b;
//     }
    
//     // Clamp final color to [0, 1]
//     final_color.r = (final_color.r > 1.0) ? 1.0 : (final_color.r < 0.0) ? 0.0 : final_color.r;
//     final_color.g = (final_color.g > 1.0) ? 1.0 : (final_color.g < 0.0) ? 0.0 : final_color.g;
//     final_color.b = (final_color.b > 1.0) ? 1.0 : (final_color.b < 0.0) ? 0.0 : final_color.b;
    
//     return final_color;
// }

/*---------------------------------------------------------------------------------------------*/ // fahd
// t_color shade_hit(t_world *world, t_comp *comp)
// {
//     t_color shade;
//     shade = (t_color){0,0,0};
//     t_light l = *(world->light);
    
//     shade = s_lighting(*(comp->obj->material), l, comp->point, comp->eyev, comp->normalv);
//     return (shade);
// }
// t_color shade_hit(t_world *world, t_comp *comp)
// {
//     t_color shade = (t_color){0, 0, 0};
//     bool shadowed;

//     shadowed = is_shadowed(world, comp->over_point);
//     shade = s_lighting(*(comp->obj->material), *(world->light), comp->over_point, comp->eyev, comp->normalv, shadowed);
//     return (shade);
// }

// t_color shade_hit(t_world *world, t_comp *comp)
// {
//     // printf("=== shade_hit ===\n");
//     // printf("Hit point: (%f, %f, %f)\n", comp->point.x, comp->point.y, comp->point.z);
//     // printf("Normal: (%f, %f, %f)\n", comp->normalv.x, comp->normalv.y, comp->normalv.z);
//     // printf("Eye vector: (%f, %f, %f)\n", comp->eyev.x, comp->eyev.y, comp->eyev.z);
//     // printf("Inside object: %d\n", comp->inside);
    
//     // Your existing shading calculations...
//     t_light l = *(world->light);
//     t_color result = s_lighting(*(comp->obj->material), l, comp->point, comp->eyev, comp->normalv);;
    
//     // printf("Final color: (%f, %f, %f)\n", result.r, result.g, result.b);
//     // printf("================\n");
//     return result;
// }

t_tuple s_position(t_stack_ray *ray, ld t)
{
    t_tuple pos;
    t_tuple scalar_res;

    s_scalar_mult(&scalar_res, ray->vect, t);
    s_add_t(&pos , ray->point, scalar_res);
    return pos;
}


// t_comp prepare_computations(t_inters *intersection, t_stack_ray *ray)
// {
//     t_comp new = {NULL, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, NULL, false};

//     if (!intersection || !ray)
//         return (new);
//     new.obj = intersection->object;
//     new.intersection = intersection;
//     new.t = intersection->inters_value;
//     new.point = s_position(ray, intersection->inters_value);
//     new.eyev = s_neg_t(ray->vect);
//     t_tuple normalv;
//     s_normal_at(&normalv, intersection->object, &new.point);
//     new.normalv = normalv;
//     if (dot_product(&normalv, &new.eyev) < 0)
//     {
//         new.inside = true;
//         new.normalv = s_neg_t(new.normalv);
//     }
//     else
//         new.inside = false;
//     t_tuple scaled_normal;
//     t_tuple over_point;
//     s_scalar_mult(&scaled_normal, new.normalv, FLT_EPSILON);
//     s_add_t(&over_point, new.point, scaled_normal);
//     new.over_point = over_point;
//     return (new);
// }


// t_comp prepare_computations(t_inters *intersection, t_stack_ray *ray)
// {
//     t_comp new = {NULL, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, NULL, false};

//     if (!intersection || !ray)
//         return (new);
//     new.obj = intersection->object;
//     new.intersection = intersection;
//     new.t = intersection->inters_value;
//     new.point = s_position(ray, intersection->inters_value);
//     new.eyev = s_neg_t(ray->vect);
    
//     t_tuple normalv;
//     s_normal_at(&normalv, intersection->object, &new.point);
//     s_vec_norm(&normalv);  // ← ADD THIS LINE - NORMALIZE THE NORMAL!
    
//     new.normalv = normalv;
//     if (dot_product(&normalv, &new.eyev) < 0)
//     {
//         new.inside = true;
//         new.normalv = s_neg_t(new.normalv);
//     }
//     else
//         new.inside = false;
    
//     t_tuple scaled_normal;
//     t_tuple over_point;
//     s_scalar_mult(&scaled_normal, new.normalv, FLT_EPSILON);
//     s_add_t(&over_point, new.point, scaled_normal);
//     new.over_point = over_point;
//     return (new);
// }


// t_comp prepare_computations(t_inters *intersection, t_stack_ray *ray)
// {
//     t_comp new = {NULL, {0, 0, 0, 0}, {0, 0, 0, 0}, 
//                   {0, 0, 0, 0}, {0, 0, 0, 0}, NULL, false};

//     if (!intersection || !ray)
//         return new;

//     new.obj = intersection->object;
//     new.intersection = intersection;
//     new.t = intersection->inters_value;
//     new.point = s_position(ray, intersection->inters_value);
//     new.eyev = s_neg_t(ray->vect);

//     // Compute normal at the point
//     t_tuple normalv;
//     s_normal_at(&normalv, intersection->object, &new.point);
//     s_vec_norm(&normalv);
//     new.normalv = normalv;

//     // Inside check
//     if (dot_product(&normalv, &new.eyev) < 0)
//     {
//         new.inside = true;
//         new.normalv = s_neg_t(new.normalv);
//     }
//     else
//         new.inside = false;

//     t_tuple scaled_normal;
//     s_scalar_mult(&scaled_normal, new.normalv, SHADOW_EPSILON);
//     s_add_t(&new.over_point, new.point, scaled_normal);

//     // t_tuple neg_scaled;
//     // s_scalar_mult(&neg_scaled, new.normalv, FLT_EPSILON);
//     // s_add_t(&new.under_point, new.point, neg_scaled);

//     // s_reflect(&new.reflectv, ray->vect, new.normalv);

//     return new;
// }
void compute_normal_at(t_tuple *normal, t_inters *intersection, t_tuple *point)
{
    if (intersection->sphere != NULL)
        s_normal_at(normal, intersection->sphere, point);
    else if (intersection->cylinder != NULL)
        s_cylinder_normal(normal, intersection->cylinder, point);
    else if (intersection->cone != NULL)
        s_cone_normal(normal, intersection->cone, point);
    else if (intersection->plane != NULL)
        s_plane_normal(normal, intersection->plane, point);
}

t_comp prepare_computations(t_inters *intersection, t_stack_ray *ray)
{
    t_comp new = {NULL, T_SPHERE, {0, 0, 0, 0}, {0, 0, 0, 0}, 
                  {0, 0, 0, 0}, {0, 0, 0, 0}, NULL, 0, false};
    
    if (!intersection || !ray)
        return new;
    if (intersection->sphere != NULL)
    {
        new.obj = intersection->sphere;
        new.obj_type = T_SPHERE;
    }
    else if (intersection->cylinder != NULL)
    {
        new.obj = intersection->cylinder;
        new.obj_type = T_CYLINDRE;
    }
    else if (intersection->cone != NULL)
    {
        new.obj = intersection->cone;
        new.obj_type = T_CONE;
    }
    else if (intersection->plane != NULL)
    {
        new.obj = intersection->plane;
        new.obj_type = T_PLAN;
    }
    new.intersection = intersection;
    new.t = intersection->inters_value;
    new.point = s_position(ray, intersection->inters_value);
    new.eyev = s_neg_t(ray->vect);
    t_tuple normalv;
    compute_normal_at(&normalv, intersection, &new.point);
    s_vec_norm(&normalv);
    if (dot_product(&normalv, &new.eyev) < 0)
    {
        new.inside = true;
        new.normalv = s_neg_t(normalv);
    }
    else
    {
        new.inside = false;
        new.normalv = normalv;
    }
    t_tuple scaled_normal;
    s_scalar_mult(&scaled_normal, new.normalv, SHADOW_EPSILON);
    s_add_t(&new.over_point, new.point, scaled_normal);
    return new;
}