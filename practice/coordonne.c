#include "colors.h"
#include "MLX42/include/MLX42/MLX42.h"

#define WIDTH 900
#define HEIGHT 900

int main(void)
{
    mlx_t *mlx = mlx_init(WIDTH, HEIGHT, "Columns", true);
    if (!mlx)
        return (1);

    mlx_image_t *image = mlx_new_image(mlx, WIDTH, HEIGHT);
	for(int a = 0; a < WIDTH; a++)
	{
		for(int b = 0; b < HEIGHT; b++)
		{
			for(int c = 0; c < WIDTH; c++)
				mlx_put_pixel(image, a, b, COLOR_WHITE);
		}	
	}

	int x = WIDTH / 2;
	while(x < WIDTH)
	{
		for(int i = 0; i < WIDTH; i++)
		{
			for(int j = 0; j < HEIGHT; j++)
			{
				if(j == WIDTH / 2)
				{
					mlx_put_pixel(image, x, j, COLOR_BLACK);
				}
			}
		}
		x++;
	}
	int y = HEIGHT / 2;
	for(int j = 0; j < HEIGHT / 2; j++)
		mlx_put_pixel(image, y, j, COLOR_BLACK);

    mlx_image_to_window(mlx, image, 0, 0);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return 0;
}
