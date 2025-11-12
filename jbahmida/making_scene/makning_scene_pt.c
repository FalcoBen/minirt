#include "../../MiniRt.h"


t_world make_world()
{

    t_world world;

    world.light = point_light((t_color *){1, 1, 1}, (t_tuple *){-10, 10, -10});
    world.obj_num = 6;
    world.objects = alloc(sizeof(t_object) * world.obj_num, false);
    for (int i = 0; i < world.obj_num; i++)
    {
        world.objects[i]->obj = alloc(sizeof(t_obj), false);
        world.objects[0]->type = T_SPHERE;
        // sphere(world.objects[i]->obj->sphere, NULL, false);
    }
    /*sphere 1 */
    sphere(world.objects[0]->obj->sphere, scaling(10, 0.01, 10), true);
    world.objects[0]->obj->sphere->material->color = create_color((ld[3]){1, 0.9, 0.9});
    world.objects[0]->obj->sphere->material->specular = 0;
    
    /*sphere 2 */
    sphere(world.objects[1]->obj->sphere, matrix_multi(translation(0, 0, 5), matrix_multi(rotation_y(-M_PI/4), rotation_x(M_PI/2))), true);
    world.objects[1]->obj->sphere->material->color = create_color((ld[3]){1, 0.9, 0.9});
    world.objects[1]->obj->sphere->material->specular = 0;

    /*sphere 3 */
    sphere(world.objects[1]->obj->sphere, matrix_multi(translation(0, 0, 5), matrix_multi(rotation_y(M_PI/4), rotation_x(M_PI/2))), true);
    world.objects[2]->obj->sphere->material->color = create_color((ld[3]){1, 0.9, 0.9});

    /*sphere 4 */
    sphere(world.objects[3]->obj->sphere, translation(-0.5, 1, 0.5), true);
    world.objects[3]->obj->sphere->material->color = create_color((ld[3]){0.1, 1, 0.1});
    world.objects[3]->obj->sphere->material->diffuse = 0.7;
    world.objects[3]->obj->sphere->material->specular = 0.3;

    /*sphere 5 */
    sphere(world.objects[4]->obj->sphere, matrix_multi(translation(1.5, 0.5, -0.5), scaling(0.5, 0.5, 0.5)), true);
    world.objects[4]->obj->sphere->material->color = create_color((ld[3]){0.5, 1, 0.5});
    world.objects[4]->obj->sphere->material->diffuse = 0.7;
    world.objects[4]->obj->sphere->material->specular = 0.3;
    /*sphere 6 */
    sphere(world.objects[5]->obj->sphere, matrix_multi(translation(-1.5, 0.33, 0.75), scaling(0.33, 0.33, 0.33)), true);
    world.objects[5]->obj->sphere->material->color = create_color((ld[3]){1, 0.8, 0.1});
    world.objects[5]->obj->sphere->material->diffuse = 0.7;
    world.objects[5]->obj->sphere->material->specular = 0.3;

}



// int main()
// {
//     t_world w = make_world();
// }