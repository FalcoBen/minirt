#include "../../MiniRt.h"

// typedef struct s_camera
// {
//     ld hsize;
//     ld vsize;
//     ld field_ov;
//     ld pixel_size;
//     ld aspect;
//     ld half_width;
//     ld half_height;
//     t_matrix *tran;
//     t_matrix *tran_inv;
// }t_camera;

void write_pixel(void *c)
{
    // int fd = open("scene.txt", O_CREAT | O_TRUNC | 0644);
    // if (fd < 0)
    //     exit(10);
    t_canva *canva = (t_canva *)c;
    t_camera *cam = camera(canva->image->width, canva->image->height, M_PI/2);
    int count = 0;
    mode_switch(2);
    for (ld y = 0; y < HEIGHT; y+=1)
    {
        for(ld x = 0; x < WIDTH; x+=1)
        {
            t_ray *ray = ray_for_camera(cam, x, y);
            t_color *color = color_at(canva->world, ray);
            int a = (y * WIDTH + x) * BPP;
            printf("=====> %d\n", a);
            // dprintf(fd, "{%Lf, %Lf} ======> {r = %Lf, g = %Lf, b = %Lf}\n", x, y, color->r, color->g, color->b);
            *((int32_t *)(canva->image->pixels + a)) = combine_color(*color);
            alloc(0, true);
        }
    }
    printf("done\n");
    // mode_switch(1);
    // alloc(0, true);


}

t_ray *ray_for_camera(t_camera *cam, ld x, ld y)
{
    ld xoffest = (x + 0.5) * cam->pixel_size;
    ld yoffset = (y + 0.5) * cam->pixel_size;
    ld world_x = cam->half_width - xoffest;
    ld world_y = cam->half_height - yoffset;
    t_tuple *pixel = matrix_to_tuple(matrix_tuple(cam->tran_inv,
        tuple_constractor(world_x, world_y, -1, 1)));
    t_tuple *origin = matrix_to_tuple(matrix_tuple(cam->tran_inv,
        tuple_constractor(0, 0, 0, 1)));
    t_tuple *direction = vec_norm(sub_t(pixel, origin));
    return (ray_constrctor(origin, direction));
}

/*
* hsize :  the horizontal size (in pixels) of the canvas that the picture will be
*   rendered to.
* vsize : is the canvas’s vertical size (in pixels).
* field_ov : angle that describes how much the camera can see
*/
t_camera *camera(ld hsize, ld vsize, ld field_ov)
{
    t_camera *new;
    ld half_view = tanl(field_ov/2);
    new = alloc(sizeof(t_camera), false);
    new->hsize = hsize;
    new->vsize = vsize;
    new->field_ov = field_ov;
    new->tran = identity_matrix(4, 4);
    new->tran_inv = matrix_inverte(new->tran);
    new->pixel_size = (half_view * 2) / hsize;
    new->aspect = hsize / vsize;
    if (new->aspect >= 1)
    {
        new->half_width = half_view;
        new->half_height = half_view / new->aspect;
    }
    else
    {
        new->half_width = new->aspect * half_view;
        new->half_height = half_view;
    }
    return new;
}

// char mode_switch(int t)
// {
//     static char c;
//     if (t == 1)
//         c = 's';
//     else if (t == 2)
//         c = 'd';
//     return c;
// }

// int main()
// {
//     // struct rlimit limit;

//     // limit.rlim_cur = limit.rlim_max = (rlim_t)2 * 1024 * 1024 * 1024;
//     // if (setrlimit(RLIMIT_AS, &limit) != 0) {
//     //     perror("setrlimit RLIMIT_AS failed");
//     //     return 1;
//     // }
//     // limit.rlim_cur = limit.rlim_max = (rlim_t)2 * 1024 * 1024 * 1024;
//     // if (setrlimit(RLIMIT_DATA, &limit) != 0) {
//     //     perror("setrlimit RLIMIT_DATA failed");
//     //     return 1;
//     // }
//     mlx_t *mlx;
//     mlx = mlx_init(WIDTH, HEIGHT, "canavas", true);
//     if (!mlx)
//     {
//         printf("%s", mlx_strerror(mlx_errno));
//         return (EXIT_FAILURE);
//     }
//     mode_switch(1);
//     t_canva *canva = create_canvas(mlx, WIDTH, HEIGHT);
//     canva->world = world();
//     // mlx_loop_hook(mlx, write_pixel, (void *)canva);
//     write_pixel((void *)canva);
//     mlx_image_to_window(canva->mlx, canva->image, 0, 0);
//     mlx_loop(mlx);
// 	mlx_terminate(mlx);
// 	return (EXIT_SUCCESS);
// }