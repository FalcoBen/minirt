#include "../../MiniRt.h"

#define wall_size 7
// #define pixel_size wall_size / 1000
// #define half wall_size / 2
#define wall_z 10
int32_t combine_color(t_color color){return (color.r << 24 | color.g << 16 | color.b << 8 | 255);}



void calculation()
{
    // t_canva *canva = (t_canva *)ptr;
    t_color color = {224,0,0};
    t_sphere *s = sphere();
    ld coor_origin[3] = {0, 0, -5};
    t_tuple *origin = create_point(coor_origin);
    ld coor[3];
    coor[2] = (ld)wall_z;
    ld world_y = 0;
    ld world_x = 0;
    ld half = (ld)7 / (ld)2;
    ld pixel_size = (ld)7 / (ld)1000;
    for (ld y = 0; y < 1000; y+=1)
    {
        world_y = (half - (pixel_size * y));
        for(ld x = 0; x < 1000; x+=1)
        {
            world_x = (-half + (pixel_size * x));
            coor[0] = world_x;
            coor[1] = world_y;
            // printf("v1 == %Lf || v2 == %Lf\n", pixel_size * y, pixel_size * x);
            // printf("world_x == %Lf || world_y == %Lf\n", world_x, world_y);
            coor[2] = wall_z;
            t_tuple *position = create_point(coor);
            t_ray *ray = ray_constrctor(origin, vec_norm(sub_t(position, origin)));
            t_intersections *xs = intersect(ray, s);
            // printf("====> %p              ||    %p\n", xs->inters_list, *xs->inters_list);
            if (hit(xs))
            {
                printf("%Lf || %Lf\n", x, y);
                printf("**********************************************\n");
            }
            free(position);
            free(ray);
            free(xs);
        }
    }
}

// int main()
// {
//     calculation();   
// }