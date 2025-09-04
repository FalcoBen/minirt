#include "minirt.h"

void	start_using_mlx(t_scene *scene)
{
	mlx_t *mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
    if (!mlx) 
		exit_error("MLX initialization failed", NULL);
    mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
		exit_error("MLX image creation failed", NULL);

    // uint32_t color = (scene->ambient_light->color_ambient_light->r << 24) |
    //                  (scene->ambient_light->color_ambient_light->g << 16) |
    //                  (scene->ambient_light->color_ambient_light->b << 8) | 0xFF;
    for (int y = 0; y < HEIGHT; y++)
	{
        for (int x = 0; x < WIDTH; x++)
            mlx_put_pixel(img, x, y, COLOR_WHITE);
	}

    if (!mlx_image_to_window(mlx, img, 0, 0))
		perror("mlx_image_to_window");
    mlx_loop(mlx);

    mlx_delete_image(mlx, img);
    mlx_terminate(mlx);
}