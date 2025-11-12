#include "../../MiniRt.h"

t_canva *create_canvas(mlx_t *p, unsigned long width, unsigned long height){
    t_canva *canva;

    canva = alloc(sizeof(t_canva), false);
    canva->mlx = p;
    // canva->width = width;
    // canva->height = height;
    canva->image =  mlx_new_image(p, width, height);
    return canva;
}

int t_x(ld x) {
    return (int)(((x + 300) * WIDTH) / 600);
}

int t_y(ld y) {
    return (int)(((300 - y) * HEIGHT) / 600);
}

// #define wall_size 7
// #define pixel_size wall_size / 1000
// #define half wall_size / 2
#define wall_z 10
// int combine_color(t_color color)
// {
//     color.r *= 255;
//     color.g *= 255;
//     color.b *= 255;
//     return (255 << 24) | ((int)color.r << 16) | ((int)color.g << 8) | (int)color.b;
// }


// void write_pixel(void *ptr)
// {
//     int fd = open("result.txt", O_CREAT | O_WRONLY, 0644);
//     if(fd < 0)
//         exit(1);
//     // int fd1 = open("position.txt", O_CREAT | O_WRONLY, 0644);
//     // if(fd1 < 0)
//     //     exit(1);
//     // int fd2 = open("specular.txt", O_CREAT | O_WRONLY, 0644);
//     // if(fd2 < 0)
//     //     exit(1);
//     /*************************************************************/
//     t_canva *canva = (t_canva *)ptr;
//     t_color color = {1, 0.2, 1};
//     t_sphere *s;
//     sphere(&s, NULL, false);
//     ld coor_origin[3] = {0, 0, -5};
//     t_tuple origin = s_create_tuple(coor_origin, 1);
//     ld coor[3];
//     coor[2] = (ld)wall_z;
//     ld world_y = 0;
//     ld world_x = 0;
//     ld half = (ld)7 / (ld)2;
//     ld pixel_size = (ld)7 / (ld)WIDTH;
//     /*************************************************************/

//     // t_intersections xs;
//     // sphere(&s, scaling(0.2, 0.2, 0.2), true);
//     /**
//      * m->ambient
//      * m->diffuse
//      * m->specular
//      * m->shininess 
//      */
//     ld m_coor[4] = {0.1, 0.9, 0.9, 50};
//     t_color m_color = {1, 0.2, 1};
//     s->material = material(m_coor, &m_color);
//     t_tuple light_position = {10, -10, 10};
//     t_color light_color = {1, 1, 1};
//     t_light *light = point_light(&light_color, &light_position);
//     /*************************************************************/
//     t_tuple ray_normal;
//     t_stack_intersections xs;
//     xs = (t_stack_intersections){false, -1, {0, 0}};
//     t_stack_ray ray;
//     t_tuple position;
//     t_inters h;
//     t_shear cp = {1, 0, 0, 0, 0, 0};
//     t_tuple point;
//     t_tuple point1;
//     point = (t_tuple){0, 0, 0, 1};
//     point1 = (t_tuple){0, 0, 0, 1};
//     t_tuple normal;
//     t_tuple eye;
    
//     char mode = mode_switch(2);
//     int count = 0;
//     for (ld y = 0; y < HEIGHT; y+=1)
//     {
//         world_y = (half - (pixel_size * y));
//         for(ld x = 0; x < WIDTH; x+=1)
//         {
//             world_x = (-half + (pixel_size * x));
//             coor[0] = world_x;
//             coor[1] = world_y;
//             coor[2] = wall_z;
//             position =  s_create_tuple(coor, 1);
//             s_sub_t(&ray_normal, position, origin);
//             s_vec_norm(&ray_normal);
//             ray = s_ray_constrctor(&origin, &ray_normal);  
//             s_intersect(&xs, ray, s);
//             h = s_hit(xs);
            
//             if (h.existence)
//             {
//                 s_scalar_mult(&point1, ray.vect, h.inters_value);
//                 s_add_t(&point, ray.point, point1);
//                 s_normal_at(&normal, s, point);
//                 eye = s_neg_t(ray.vect);
//                 t_color p_color = s_lighting(*(s->material), *light, point, eye, normal);
//                 dprintf(fd,"red = %f || green = %f || blue = %f\n", color.r, color.g, color.b);
//                 // int a = (y * WIDTH + x) * BPP;
//                 // *((int32_t *)(canva->image->pixels + a)) = combine_color(p_color);
//             }
//             else
//             {
//                 // int a = (y * WIDTH + x) * BPP;
//                 // *((int32_t *)(canva->image->pixels + a)) = combine_color((t_color){0,0,0});
//             }
//         }
//     }
// }

char mode_switch(int t)
{
    static char c;
    if (t == 1)
        c = 's';
    else if (t == 2)
        c = 'd';
    return c;
}
// int main()
// {
//     struct rlimit rl;
    
//     /* Limit virtual memory (heap + mmap etc.) to 1 GiB */
//     rl.rlim_cur = rl.rlim_max = (rlim_t)2 << 30;  // 1 GiB
//     if (setrlimit(RLIMIT_AS, &rl) != 0) {
//         perror("setrlimit RLIMIT_AS");
//     }
    
//     /* Limit stack size to 1 GiB */
//     rl.rlim_cur = rl.rlim_max = (rlim_t)2 << 30;  // 1 GiB
//     if (setrlimit(RLIMIT_STACK, &rl) != 0) {
//         perror("setrlimit RLIMIT_STACK");
//     }
//     mode_switch(1);
//     mlx_t *mlx;
//     mlx = mlx_init(WIDTH, HEIGHT, "canavas", true);
//     if (!mlx)
//     {
//         printf("%s", mlx_strerror(mlx_errno));
//         return (EXIT_FAILURE);
//     }
//     t_canva *canva = create_canvas(mlx, WIDTH, HEIGHT);
//     // mlx_loop_hook(mlx, write_pixel, (void *)canva);
//     write_pixel((void *)canva);
//     mlx_image_to_window(canva->mlx, canva->image, 0, 0);
//     mlx_loop(mlx);
//     alloc(0, true, 's');
// 	mlx_terminate(mlx);
// 	return (EXIT_SUCCESS);
// }