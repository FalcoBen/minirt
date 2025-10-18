#include "minirt.h"


int number_of_all_objects(t_scene *scene)
{
    int counter = 0;
    t_sphere *sp = scene->sphere;
    t_plane *pl = scene->plane;
    t_cylinder *cy = scene->cylinder;
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
    return counter;
}
double  switch_ld(int color)
{
    return (color/255.0);
}
// printf("--------------------%d--------------------------\n", world.obj_num);
// exit(1223);

void obj_creator(t_obj *obj, t_scene *scene, t_type type)
{
    if (!obj)
        return ;


    if (type == SPHERE)
    {
        t_sphere_ja *dest;
        dest = obj->sphere_ja;

        t_sphere *src;
        src = scene->sphere;
        
        t_ambient_light *ambient_light;
        ambient_light = scene->ambient_light;
        dest->material->color_ja = malloc(sizeof(t_color_ja));
        static int id = 0;

        dest->origin = malloc(sizeof(t_tuple));
        dest->origin->x = src->coor_sphere->x;
        dest->origin->y = src->coor_sphere->y;
        dest->origin->z = src->coor_sphere->z;
        // dest->origin->w = src->coor_sphere->w;
        dest->material->diffuse =  ambient_light->bright_ambient_light;
        dest->radius = src->diameter_sphere / 2;
        dest->material->color_ja->r = switch_ld(src->color_sphere->r);
        dest->material->color_ja->g = switch_ld(src->color_sphere->g);
        dest->material->color_ja->b = switch_ld(src->color_sphere->b);
        dest->id = id;
        id++;
    }

    // if (type == PLANE)
    // {
    //     t_plane_ja *dest;
    //     t_plane *src;

    //     dest = obj->plane_ja;
    //     src = (t_plane *)(object);

    //     static int id = 0;

    //     dest->origin = malloc(sizeof(t_tuple));
    //     dest->origin->x = src->coor_plane->x;
    //     dest->origin->y = src->coor_plane->y;
    //     dest->origin->z = src->coor_plane->z;
    //     dest->origin->w = src->coor_plane->w;

    //     dest->radius = src->diameter_plane / 2;
    //     dest->id = id;
    //     id++;
    // }

    // if (type == CYLINDER)
    // {
    //     t_cylinder_ja *dest;
    //     t_cylinder *src;

    //     dest = obj->cylinder_ja;
    //     src = (t_cylinder *)(object);

    //     static int id = 0;

    //     dest->origin = malloc(sizeof(t_tuple));
    //     dest->origin->x = src->coor_cylinder->x;
    //     dest->origin->y = src->coor_cylinder->y;
    //     dest->origin->z = src->coor_cylinder->z;
    //     dest->origin->w = src->coor_cylinder->w;

    //     dest->radius = src->diameter_cylinder / 2;
    //     dest->id = id;
    //     id++;
    // }

}


t_world s_world(t_scene *scene)
{
    t_world     world;
    // t_sphere    *sphere;
    int         i;
    static int  id = 0;

    world.obj_num = number_of_all_objects(scene);
    world.objects = malloc(sizeof(t_object_ja *) * (world.obj_num + 1));

    // sphere = scene->sphere;
    i = 0;
    while (/*sphere &&*/ i < world.obj_num)
    {
        world.objects[i] = malloc(sizeof(t_object_ja));
        world.objects[i]->obj = malloc(sizeof(t_obj));
        world.objects[i]->obj->sphere_ja = malloc(sizeof(t_sphere_ja));

        world.objects[i]->obj->sphere_ja->material = malloc(sizeof(t_material));

        world.objects[i]->type = SPHERE;

        obj_creator(world.objects[i]->obj, scene, SPHERE);

        world.objects[i]->obj->sphere_ja->id = id;
        scene->sphere = scene->sphere->next;
        id++;
        i++;
    }

    /*-----------------------------plane----------------------------------*/
    
    // t_plane *plane = scene->plane;
    // while (plane && i < world.obj_num)
    // {
        //     world.objects[i] = malloc(sizeof(t_object_ja));
        //     world.objects[i]->obj = malloc(sizeof(t_obj));
        //     world.objects[i]->obj->plane_ja = malloc(sizeof(t_plane_ja));
        
        //     world.objects[i]->obj->plane_ja->material = malloc(sizeof(t_material));
        
        //     world.objects[i]->type = PLANE;
        
        //     obj_creator(world.objects[i]->obj, plane, PLANE);
        
        //     world.objects[i]->obj->plane_ja->id = id;
        //     // printf("****************%d****************\n", world.objects[i]->obj->sphere_ja->id);
        
        //     id++;
        //     plane = plane->next;
        //     i++;
        // }
        
    /*-----------------------------cylinder----------------------------------*/

    // t_cylinder *cylinder = scene->cylinder;
    // while (cylinder && i < world.obj_num)
    // {
    //     world.objects[i] = malloc(sizeof(t_object_ja));
    //     world.objects[i]->obj = malloc(sizeof(t_obj));
    //     world.objects[i]->obj->cylinder_ja = malloc(sizeof(t_cylinder_ja));

    //     world.objects[i]->obj->cylinder_ja->material = malloc(sizeof(t_material));

    //     world.objects[i]->type = CYLINDER;

    //     obj_creator(world.objects[i]->obj, cylinder, CYLINDER);

    //     world.objects[i]->obj->cylinder_ja->id = id;
    //     printf("****************%d****************\n", world.objects[i]->obj->cylinder_ja->id);

    //     id++;
    //     cylinder = cylinder->next;
    //     i++;
    // }

    // Optional: mark the end (useful for iteration)
    world.objects[i] = NULL;

    return (world);
}



void    print_world(t_world *world)
{
    int i = 0;
    puts("::::::::::::::::::::::::::::::::::::::::;;\n");
    while(i < world->obj_num)
    {
        if(world->objects[i]->type == SPHERE)
        {
            printf("id = [%d]\n", world->objects[i]->obj->sphere_ja->id);
            t_tuple *coor = world->objects[i]->obj->sphere_ja->origin;
            printf("coor X = %f\n", coor->x);
            printf("coor Y = %f\n", coor->y);
            printf("coor Z = %f\n", coor->z);
            printf("coor W = %f\n", coor->w);
            printf("radius = [%f]\n", world->objects[i]->obj->sphere_ja->radius);
            t_color_ja *color = world->objects[i]->obj->sphere_ja->material->color_ja;
            printf("color r = %f\n", color->r);
            printf("color g = %f\n", color->g);
            printf("color b = %f\n", color->b);
        }
        i++;
    }
}
