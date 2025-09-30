#include "rays.h"

void exit_error(const char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

int main()
{
    mlx_t *mlx = mlx_init(WIDTH, HEIGHT, "Gradient Ray Tracer", false);
    if (!mlx)
        exit_error("MLX initialization failed");

    mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
    {
        mlx_terminate(mlx);
        exit_error("MLX image creation failed");
    }

    render_mlx_gradient(mlx, img);

    if (mlx_image_to_window(mlx, img, 0, 0) < 0)
    {
        mlx_delete_image(mlx, img);
        mlx_terminate(mlx);
        exit_error("Failed to display image");
    }

    mlx_loop(mlx);
    mlx_delete_image(mlx, img);
    mlx_terminate(mlx);
    return 0;
}