#include "../../MiniRt.h"

int g_fd;
t_canva s_canva_create(mlx_t *p, int w, int h)
{
    t_canva local;

    local = (t_canva){NULL,0,0};
    local.mlx = p;
    local.image = mlx_new_image(p, w, h);
    return local;
}
#define wall_z 0.1
int combine_color(t_color color)
{
    color.r = (color.r > 1) ? 1 : (color.r < 0) ? 0 : color.r;
    color.g = (color.g > 1) ? 1 : (color.g < 0) ? 0 : color.g;
    color.b = (color.b > 1) ? 1 : (color.b < 0) ? 0 : color.b;
    return (255 << 24) | ((int)(color.r * 255) << 16) | ((int)(color.g * 255) << 8) | (int)(color.b * 255);
}
void pixels(void *p)
{
    int fd = open("pixels_output_mac", O_CREAT | O_WRONLY, 0644);
    if (fd < 0)
    {
        perror("open failed");
        exit(12);
    }
    g_fd = fd;
    int fd1 = open("delta.txt", O_CREAT | O_WRONLY, 0644);
    if (fd1 < 0)
        exit(12);
    t_canva *canva = (t_canva *)p;
    t_sphere *s = NULL;
    t_matrix *scale = scaling(10, 0.01, 10);
    t_matrix *rx = rotation_x(M_PI/2);
    t_matrix *ry = rotation_y(-M_PI/4);
    t_matrix *trans = translation(0, 0, 5);
    
    t_matrix *tmp = matrix_multi(rx, scale);

    t_matrix *tmp1 = matrix_multi(ry, tmp);

    t_matrix *tran1 = matrix_multi(trans, tmp1);
    sphere(&s, NULL, false);
    sphere(&s, tran1, true);
    // t_matrix *tran = matrix_multi(translation(0.7, 0, 0), scaling(3,3,1));
    
    // sphere(&s, tran, true);
    ld coor_origin[3] = {0, 0, -5};
    t_tuple origin = s_create_tuple(coor_origin, 1);
    ld coor[3];
    coor[2] = (ld)wall_z;
    ld world_y = 0;
    ld world_x = 0;
    ld half = (ld)10 / (ld)2;
    ld pixel_size = (ld)10 / (ld)WIDTH;
    /*************************************************************/
    ld m_coor[4] = {0.1, 0.9, 0.9, 200};
    ld m_color[3] = {1, 0.2, 1};
    s->material = material();
    s->material->color = create_color(m_color);
    s->material->ambient = 0.1;
    s->material->diffuse = 0.9;
    s->material->specular = 0.9;
    s->material->shininess = 200;
    /*************************************************************/
    t_tuple light_position = {-10, 10, -10};
    t_color light_color = {1, 1, 1};
    t_light *light = point_light(&light_color, &light_position);
    /*************************************************************/
    t_tuple ray_normal;
    t_stack_intersections xs;
    xs = (t_stack_intersections){false, -1, {0, 0}};
    t_stack_ray ray;
    t_tuple position;
    t_inters h;
    t_shear cp = {1, 0, 0, 0, 0, 0};
    t_tuple point = {0,0,0,1};
    t_tuple point1 = {0,0,0,1};
    point = (t_tuple){0, 0, 0, 1};
    point1 = (t_tuple){0, 0, 0, 1};
    t_tuple normal;
    normal = (t_tuple){0,0,0,0};
    t_tuple eye;
    static int counter = 0;
    /*************************************************************/
    for (ld y = 0; y < HEIGHT; y+=1)
    {
        world_y = (half - (pixel_size * y));
        for(ld x = 0; x < WIDTH; x+=1)
        {
            
            world_x = (-half + (pixel_size * x));
            coor[0] = world_x;
            coor[1] = world_y;
            coor[2] = wall_z;
            position =  s_create_tuple(coor, 1);
            s_sub_t(&ray_normal, position, origin);
            s_vec_norm(&ray_normal);
            ray = s_ray_constrctor(&origin, &ray_normal);  
            s_intersect(&xs, ray, s);
            h = s_hit(xs);

            if (h.existence)
            {
                s_scalar_mult(&point1, ray.vect, h.inters_value);
                s_add_t(&point, ray.point, point1);
                /*we have the issue inside s_normal_at*/
                s_normal_at(&normal, s, &point);
                
                eye = s_neg_t(ray.vect);
                t_color p_color = s_lighting(*(s->material), *light, point, eye, normal);
                // dprintf(fd,"{%f, %f} |==> ", x, y);
                // dprintf(fd,"red = %f || green = %f || blue = %f\n", p_color.r, p_color.g, p_color.b);
                printf("{%f, %f}\n", x, y);
                int a = (y * WIDTH + x) * BPP;
                *((int32_t *)(canva->image->pixels + a)) = combine_color(p_color);
            }
            else
            {
                int a = (y * WIDTH + x) * BPP;
                printf("==========================>{%f, %f}\n", x, y);
                *((int32_t *)(canva->image->pixels + a)) = combine_color((t_color){0,0,0});
            }
        }
        
    }
    alloc(0, true);
    return ;
}

int main()
{
    // struct rlimit rl;
    
    /* Limit virtual memory (heap + mmap etc.) to 1 GiB */
    // rl.rlim_cur = rl.rlim_max = (rlim_t)2 << 30;  // 1 GiB
    // if (setrlimit(RLIMIT_AS, &rl) != 0) {
    //     perror("setrlimit RLIMIT_AS");
    // }
    
    // /* Limit stack size to 1 GiB */
    // rl.rlim_cur = rl.rlim_max = (rlim_t)1 << 30;  // 1 GiB
    // if (setrlimit(RLIMIT_STACK, &rl) != 0) {
    //     perror("setrlimit RLIMIT_STACK");
    // }
    
    mlx_t *mlx;
    mlx = mlx_init(WIDTH, HEIGHT, "stack", true);
    if (!mlx)
    {
        printf("%s", mlx_strerror(mlx_errno));
        return (EXIT_FAILURE);
    }
    t_canva canva = s_canva_create(mlx, WIDTH, HEIGHT);
    // mlx_loop_hook(mlx, write_pixel, (void *)canva);
    pixels((void *)&canva);
    mlx_image_to_window(canva.mlx, canva.image, 0, 0);
    mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
    
}

// int main()
// {
//     ld arr[16] = {  1 , 2 , 3 , 4 ,
//                     5 , 6 , 7 , 8 ,
//                     9 , 8 , 7 , 6 ,
//                     5 , 4 , 3 , 2
//                 };
//     ld arr1[16] = {-2 , 1 , 2 , 3 ,
//                     3 , 2 , 1 , -1 ,
//                     4 , 3 , 6 , 5 ,
//                     1 , 2 , 7 , 8};
//     t_matrix m1 = s_matrix_constractor(4, 4, arr);
//     t_matrix m2 = s_matrix_constractor(4, 4, arr1);
//     t_matrix m = s_matrix_multi(&m1, &m2);

//     for(int i = 0; i < m.row; i++)
//     {
//         for(int j = 0; j < m.col; j++)
//             printf("%f, ", m.stack_matrix[i][j]);
//         printf("\n");
//     }
//     return 0;
// }