#include "../MiniRt.h"
#include "../../fbenalla/miniRT/parsing.h"

void    assign_spher_object(t_obj *obj, t_sphere_fb *src, t_ambient_light_fb *ambient_light)
{
    obj->sphere->material->color->r = src->color_sphere->r / 255.0;
    obj->sphere->material->color->g = src->color_sphere->g / 255.0;
    obj->sphere->material->color->b = src->color_sphere->b / 255.0;
    obj->sphere->material->ambient = ambient_light->bright_ambient_light_fb;
    obj->sphere->material->diffuse = 0.9;
    obj->sphere->material->specular = 0.9;
    obj->sphere->material->shininess = 200.0;
    if (src->flag_bump && src->bump_texture)
    {
        if (src->bump_texture->type == 1)
        {
            obj->sphere->material->has_color_texture = true;
            obj->sphere->material->color_texture = src->bump_texture;
            obj->sphere->material->has_bump_map = false;
        }
        else if (src->bump_texture->type == 2)
        {
            obj->sphere->material->has_bump_map = true;
            obj->sphere->material->bump_map = src->bump_texture;
            obj->sphere->material->bump_strength = 0.3;
            obj->sphere->material->has_color_texture = false;
        }
    }
    else 
    {
        obj->sphere->material->has_color_texture = false;
        obj->sphere->material->has_bump_map = false;
    }
}

void obj_creator_sphere(t_obj *obj, t_sphere_fb *src, t_ambient_light_fb *ambient_light)
{
    ld          radius;
    t_matrix    *trans;

    
    if (!obj || !src)
        return ;
    sphere(&obj->sphere, NULL, false);
    
    radius = src->diameter_sphere / 2.0;
    
    trans = translation(src->coor_sphere->x, 
                                  src->coor_sphere->y, 
                                  src->coor_sphere->z);
    t_matrix *scale = scaling(radius, radius, radius);
    t_matrix *transform = matrix_multi(trans, scale);
    sphere(&obj->sphere, transform, true);
    assign_spher_object(obj, src, ambient_light);

    copy_matrix_contant(obj->sphere->inv);
    copy_matrix_contant(obj->sphere->transform);
}
void    assigne_cylinder_object(t_obj *obj, t_cylinder_fb *src, t_ambient_light_fb *ambient_light)
{
    obj->cylinder->material->color->r = src->color_cylinder->r / 255.0;
    obj->cylinder->material->color->g = src->color_cylinder->g / 255.0;
    obj->cylinder->material->color->b = src->color_cylinder->b / 255.0;
    obj->cylinder->material->ambient = ambient_light->bright_ambient_light_fb;
    obj->cylinder->material->diffuse = 0.9;
    obj->cylinder->material->specular = 0.9;
    obj->cylinder->material->shininess = 200.0;
    obj->cylinder->minimum = 0.0;
    obj->cylinder->maximum = src->height_cylinder_fb;
    obj->cylinder->closed = src->closed;
    if (src->bump_texture && src->bump_texture->type == 1)
    {
        obj->cylinder->material->has_color_texture = true;
        obj->cylinder->material->color_texture = src->bump_texture;
        obj->cylinder->material->has_bump_map = false;
    }
    else if (src->bump_texture && src->bump_texture->type == 2 && src->bump_texture->image)
    {
        obj->cylinder->material->has_color_texture = false;
        obj->cylinder->material->color_texture = src->bump_texture;
        obj->cylinder->material->has_bump_map = true;
        obj->cylinder->material->bump_map = src->bump_texture;
        // obj->cylinder->material->bump_strength = 0.2;
    }
    else
    {
        obj->cylinder->material->has_bump_map = false;
        obj->cylinder->material->bump_map = NULL;
        obj->cylinder->material->has_color_texture = false;
        obj->cylinder->material->color_texture = NULL;
    }
}
void    assigne_cone_object(t_obj *obj, t_cone_fb *src, t_ambient_light_fb *ambient_light)
{
    obj->cone->material->color->r = src->color_cone->r / 255.0;
    obj->cone->material->color->g = src->color_cone->g / 255.0;
    obj->cone->material->color->b = src->color_cone->b / 255.0;
    obj->cone->material->ambient = ambient_light->bright_ambient_light_fb;
    obj->cone->material->diffuse = 0.9;
    obj->cone->material->specular = 0.9;
    obj->cone->material->shininess = 200.0;
    obj->cone->minimum = src->minimum;
    obj->cone->maximum = src->maximum;
    obj->cone->closed = src->closed_flag;
    if (src->flag_bump && src->bump_texture)
    {
        if (src->bump_texture->type == 1)
        {
            obj->cone->material->has_color_texture = true;
            obj->cone->material->color_texture = src->bump_texture;
            obj->cone->material->has_bump_map = false;
        }
        else if (src->bump_texture->type == 2)
        {
            obj->cone->material->has_bump_map = true;
            obj->cone->material->bump_map = src->bump_texture;
            // obj->cone->material->bump_strength = 0.3;
            obj->cone->material->has_color_texture = false;
        }
    }
    else 
    {
        obj->cone->material->has_color_texture = false;
        obj->cone->material->has_bump_map = false;
    }
}
void    assigne_plane_object(t_obj *obj, t_plane_fb *src, t_ambient_light_fb *ambient_light)
{
    obj->plane->material->color->r = src->color_plane->r / 255.0;
    obj->plane->material->color->g = src->color_plane->g / 255.0;
    obj->plane->material->color->b = src->color_plane->b / 255.0;
    obj->plane->material->ambient = ambient_light->bright_ambient_light_fb;
    obj->plane->material->diffuse = 0.9;
    obj->plane->material->specular = 0.0;
    obj->plane->material->shininess = 200.0;
    
    if (src->flag_bump && src->bump_texture) 
    {
       if (src->bump_texture->type == 1)
       {
            obj->plane->material->has_color_texture = true;
            obj->plane->material->color_texture = src->bump_texture;
            obj->plane->material->has_bump_map = false;
            obj->plane->material->bump_strength = 0.1;
        } 
        else if (src->bump_texture->type == 2)
        {
            obj->plane->material->has_bump_map = true;
            obj->plane->material->bump_map = src->bump_texture;
            obj->plane->material->has_color_texture = false;
            src->bump_texture->scale = 1.0;
        }
    }
    else
    {
        obj->plane->material->has_color_texture = false;
        obj->plane->material->has_bump_map = false;
    }
}
void    assign_coor_and_vects_cy(t_tuple *origin, t_tuple *normal, t_cylinder_fb *src)
{
    origin->x = src->coor_cylinder->x;
    origin->y = src->coor_cylinder->y;
    origin->z = src->coor_cylinder->z;
    origin->w = 1;
    normal->x = src->vector_cylinder->x;
    normal->y = src->vector_cylinder->y;
    normal->z = src->vector_cylinder->z;
    normal->w = 0;
}
void    assign_coor_and_vects_cone(t_tuple *origin, t_tuple *normal, t_cone_fb *src)
{
    origin->x = src->coor_cone->x;
    origin->y = src->coor_cone->y;
    origin->z = src->coor_cone->z;
    origin->w = 1;
    normal->x = src->vector_cone->x;
    normal->y = src->vector_cone->y;
    normal->z = src->vector_cone->z;
    normal->w = 0;
}
void    assign_coor_and_vects_pl(t_tuple *origin, t_tuple *normal, t_plane_fb *src)
{
    origin->x = src->coor_plane->x;
    origin->y = src->coor_plane->y;
    origin->z = src->coor_plane->z;
    origin->w = 1;
    
    normal->x = src->vector_plane->x;
    normal->y = src->vector_plane->y;
    normal->z = src->vector_plane->z;
    normal->w = 0;
}

void obj_creator_cylinder(t_obj *obj, t_cylinder_fb *src, t_ambient_light_fb *ambient_light)
{
    if (!obj || !src)
        return;

    cylinder(&obj->cylinder, NULL, false);
    
    ld radius = src->diameter_cylinder / 2.0;
    
    t_tuple *origin = malloc(sizeof(t_tuple));
    t_tuple *normal = malloc(sizeof(t_tuple));
    assign_coor_and_vects_cy(origin, normal, src);
    assigne_cylinder_object(obj, src, ambient_light);
    t_matrix *transform = create_cylinder_transform(origin, normal, radius, src->height_cylinder_fb);
    cylinder(&obj->cylinder, transform, true);
    copy_matrix_contant(obj->cylinder->inv);
    copy_matrix_contant(obj->cylinder->transform);
}

void obj_creator_cone(t_obj *obj, t_cone_fb *src, t_ambient_light_fb *ambient_light)
{
    if (!obj || !src)
        return;

    cone(&obj->cone, NULL, false);
    
    ld radius = src->maximum / 2.0;

    t_tuple *origin = malloc(sizeof(t_tuple));
    
    t_tuple *normal = malloc(sizeof(t_tuple));
    assign_coor_and_vects_cone(origin, normal, src);

    ld height = src->maximum - src->minimum;
    obj->cone->origin = origin;
    t_matrix *transform = create_cone_transform(origin, normal, radius, height);
    cone(&obj->cone, transform, true);
    assigne_cone_object(obj, src, ambient_light);

    copy_matrix_contant(obj->cone->inv);
    copy_matrix_contant(obj->cone->transform);
}

void obj_creator_plane(t_obj *obj, t_plane_fb *src, t_ambient_light_fb *ambient_light)
{
    if (!obj || !src)
        return;

    t_tuple *origin = malloc(sizeof(t_tuple));
    t_tuple *normal = malloc(sizeof(t_tuple));
    assign_coor_and_vects_pl(origin, normal, src);

    
    plane(&obj->plane, origin, normal);

    t_matrix *transform = create_plane_transform(origin, normal);
    obj->plane->transform = transform;
    obj->plane->inv = matrix_inverte(transform);
    assigne_plane_object(obj, src, ambient_light);
    copy_matrix_contant(obj->plane->inv);
    copy_matrix_contant(obj->plane->transform);
    free(origin);
    free(normal);
}

void validate_scene(t_scene *scene)
{
    printf("=== Validating Scene ===\n");
    
    if (!scene->camera)
    {
        fprintf(stderr, "No camera in scene!\n");
        exit(1);
    }
    printf("Camera: pos(%.2f,%.2f,%.2f) orient(%.2f,%.2f,%.2f) FOV=%.2f\n",
           scene->camera->coor_camera->x,
           scene->camera->coor_camera->y,
           scene->camera->coor_camera->z,
           scene->camera->vector_camera->x,
           scene->camera->vector_camera->y,
           scene->camera->vector_camera->z,
           scene->camera->angle_view);
    
    if (!scene->ambient_light)
    {
        fprintf(stderr, "No ambient light in scene!\n");
        exit(1);
    }
    printf("Ambient light: brightness=%.2f color(%d,%d,%d)\n",
           scene->ambient_light->bright_ambient_light_fb,
           scene->ambient_light->color_ambient_light->r,
           scene->ambient_light->color_ambient_light->g,
           scene->ambient_light->color_ambient_light->b);
    
    int sphere_count = 0;
    t_sphere_fb *sp = scene->sphere;
    while (sp)
    {
        // validate_sphere_data(sp);
        sphere_count++;
        sp = sp->next;
    }
    printf("Total spheres: %d\n", sphere_count);
    
    int plane_count = 0;
    t_plane_fb *pl = scene->plane;
    while (pl)
    {
        printf("Plane: pos(%.2f,%.2f,%.2f) normal(%.2f,%.2f,%.2f) color(%d,%d,%d)\n",
               pl->coor_plane->x, pl->coor_plane->y, pl->coor_plane->z,
               pl->vector_plane->x, pl->vector_plane->y, pl->vector_plane->z,
               pl->color_plane->r, pl->color_plane->g, pl->color_plane->b);
        plane_count++;
        pl = pl->next;
    }
    printf("Total planes: %d\n", plane_count);
    
}
void    loop_lights(t_world *world, t_light_fb *current_light, int *i)
{
     world->lights[*i] = malloc(sizeof(t_light));
     world->lights[*i]->intensity = malloc(sizeof(t_color));
     world->lights[*i]->intensity->r = current_light->color_light->r / 255.0;
     world->lights[*i]->intensity->g = current_light->color_light->g / 255.0;
     world->lights[*i]->intensity->b = current_light->color_light->b / 255.0;
     
     world->lights[*i]->intensity->r *= current_light->bright_light;
     world->lights[*i]->intensity->g *= current_light->bright_light;
     world->lights[*i]->intensity->b *= current_light->bright_light;
     
     world->lights[*i]->position = malloc(sizeof(t_tuple));
     world->lights[*i]->position->x = current_light->coor_light->x;
     world->lights[*i]->position->y = current_light->coor_light->y;
     world->lights[*i]->position->z = current_light->coor_light->z;
     world->lights[*i]->position->w = 1;
    
}

void    light_obj_creator(t_world *world, t_scene *scene)
{
    int i;
    i = 0;
    world->light_num = 0;
    t_light_fb *light_count = scene->light;
    while (light_count)
    {
        world->light_num++;
        light_count = light_count->next;
    }
    if (world->light_num > 0)
    {
        world->lights = malloc(sizeof(t_light *) * world->light_num);
        if (!world->lights)
        {
            fprintf(stderr, "Failed to allocate world->lights\n");
            exit(1);
        }
        t_light_fb *current_light = scene->light;
        while (current_light && i < world->light_num)
        {
            loop_lights(world, current_light,&i);
            current_light = current_light->next;
            i++;
        }
    }

}

void    s_world_sphere_constractor(t_world *world, t_scene *scene, int *i)
{
    t_sphere_fb *current_sphere = scene->sphere;
    while (current_sphere && *i < world->obj_num)
    {
        world->objects[*i] = malloc(sizeof(t_object));
        if (!world->objects[*i])
        {
            fprintf(stderr, "Failed to allocate object sphere %d\n", i);
            exit(1);
        }
        
        world->objects[*i]->type = T_SPHERE;
        world->objects[*i]->obj = malloc(sizeof(t_obj));
        if (!world->objects[*i]->obj)
        {
            fprintf(stderr, "Failed to allocate obj %d\n", i);
            exit(1);
        }

        obj_creator_sphere(world->objects[*i]->obj, current_sphere, scene->ambient_light);
        
        world->objects[*i]->obj->sphere->id = *i;
        
        current_sphere = current_sphere->next;
        (*i)++;
    }
}

void    s_world_plane_constractor(t_world *world, t_scene *scene, int *i)
{
    t_plane_fb *current_plane = scene->plane;
    while(current_plane && *i < world->obj_num)
    {
        world->objects[*i]= malloc(sizeof(t_object));
        if (!world->objects[*i])
        {
            fprintf(stderr, "Failed to allocate object  plane %d\n", i);
            exit(1);
        }
        world->objects[*i]->type = T_PLAN;
        world->objects[*i]->obj = malloc(sizeof(t_obj));
        if (!world->objects[*i]->obj)
        {
            fprintf(stderr, "Failed to allocate obj  plane %d\n", i);
            exit(1);
        }
        obj_creator_plane(world->objects[*i]->obj, current_plane, scene->ambient_light);
        world->objects[*i]->obj->plane->id = *i;
        
        current_plane = current_plane->next;
        (*i)++;
    }
}
void    s_world_cylinder_constractor(t_world *world, t_scene *scene, int *i)
{
    t_cylinder_fb *cuurent_cylinder = scene->cylinder;
    while(cuurent_cylinder && *i < world->obj_num)
    {
        world->objects[*i]= malloc(sizeof(t_object));
        if (!world->objects[*i])
        {
            fprintf(stderr, "Failed to allocate object  cylinder %d\n", i);
            exit(1);
        }
        world->objects[*i]->type = T_CYLINDRE;
        world->objects[*i]->obj = malloc(sizeof(t_obj));
        if (!world->objects[*i]->obj)
        {
            fprintf(stderr, "Failed to allocate obj  cylinder %d\n", i);
            exit(1);
        }
        obj_creator_cylinder(world->objects[*i]->obj, cuurent_cylinder, scene->ambient_light);
        world->objects[*i]->obj->cylinder->id = *i;
        
        cuurent_cylinder = cuurent_cylinder->next;
        (*i)++;
    }
}

void    s_world_cone_constractor(t_world *world, t_scene *scene, int *i)
{
    t_cone_fb *current_cone = scene->cone;
    while(current_cone && *i < world->obj_num)
    {
        world->objects[*i]= malloc(sizeof(t_object));
        if (!world->objects[*i])
        {
            fprintf(stderr, "Failed to allocate object  cone %d\n", i);
            exit(1);
        }
        world->objects[*i]->type = T_CONE;
        world->objects[*i]->obj = malloc(sizeof(t_obj));
        if (!world->objects[*i]->obj)
        {
            fprintf(stderr, "Failed to allocate obj  cone %d\n", i);
            exit(1);
        }
        obj_creator_cone(world->objects[*i]->obj, current_cone, scene->ambient_light);
        world->objects[*i]->obj->cone->id = *i;
        
        current_cone = current_cone->next;
        (*i)++;
    }
}
t_world s_world(t_scene *scene)
{
    t_world world;
    int i;
    light_obj_creator(&world, scene);
    world.obj_num = number_of_all_objects(scene);
    world.objects = malloc(sizeof(t_object *) * (world.obj_num + 1));
    if (!world.objects)
    {
        fprintf(stderr, "Failed to allocate world.objects\n");
        exit(1);
    }

    i = 0;
    s_world_sphere_constractor(&world, scene, &i); //
    s_world_plane_constractor(&world, scene, &i); //
    s_world_cylinder_constractor(&world, scene, &i); //
    s_world_cone_constractor(&world, scene, &i); //
    world.objects[i] = NULL;

    return world;
}
