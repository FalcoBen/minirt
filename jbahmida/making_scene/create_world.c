#include "../../MiniRt.h"

void s_intersect_world(t_stack_intersections *in, t_world w, t_stack_ray *ray)
{
    t_
}








t_color s_create_color(ld *vals)
{
    t_color color;
    
    color = (t_color){0,0,0};
    color.r = vals[0];
    color.g = vals[1];
    color.b = vals[2];
    return color;
}

t_world s_world()
{
    t_world w;
    w = (t_world){0, NULL, NULL};
    /**************************************** */
    ld p_coor[3] = {-10, 10, -10};
    ld light_color_coor[3] = {1, 1, 1};
    t_color *light_color = create_color(light_color_coor);//-----------
    t_tuple *light_position = create_point(p_coor);//------------
    ld s1_coor[4] = {0, 0.7, 0.2, 0};
    ld color_coor[3] = {0.8, 1.0, 0.6};
    t_color s1_color = s_create_color(color_coor);//-------------
    t_matrix *m = scaling(0.5, 0.5, 0.5);//-------------

    w.light = point_light(light_color, light_position);
    w.obj_num = 2;
    w.objects = alloc(sizeof(t_object *) * 3, false);


    int i = 0;
    while (i < 2)
    {
        w.objects[i] = alloc(sizeof(t_object), false);
        w.objects[i]->type = T_SPHERE;
        w.objects[i]->obj = alloc(sizeof(t_obj), false);
        sphere(&w.objects[i]->obj->sphere, NULL, false); 
        i++;
    }
    w.objects[i] = NULL;
    (w.objects[0]->obj->sphere)->material = material(s1_coor, &s1_color);
    (w.objects[1]->obj->sphere)->material = material(s1_coor, &s1_color);
    sphere(&(w.objects[1]->obj->sphere), m, true);
    return w;
}

// int main()
// {
//     t_world w = s_world();
//     ld o_coor[3] = {0,0,-5};
//     ld v_coor[3] = {0,0,1};
//     t_tuple origine = s_create_tuple(o_coor, 1);
//     t_tuple vect = s_create_tuple(v_coor, 0);
//     t_stack_ray ray = s_ray_constrctor(&origine, &vect);
//     t_stack_intersections in = (t_stack_intersections){false, 0, (t_inters){0, 0}, NULL};
//     s_intersect_world(&in, w, &ray);
//     for(int i = 0; i < in.count; i++)
//     {
//         printf("%f\n", in.world_inters_list[i].inters_value);
//     }   
// }