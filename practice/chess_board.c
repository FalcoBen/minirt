#include <stdlib.h>
#include "../MLX42/include/MLX42/MLX42.h"
#include "colors.h"

#define WIDTH 900
#define HEIGHT 900
#define CUBE 30

int main()
{
	mlx_t *mlx = mlx_init(WIDTH, HEIGHT, "Window", 1);
	if(!mlx)
		return 1;
	mlx_image_t *background = mlx_new_image(mlx, WIDTH, HEIGHT);
	int x = 0;
	int y = 0;
	while(x < WIDTH)
	{
		y = 0;
		while(y < HEIGHT)
		{
			int color;
			if(((x / CUBE) + (y / CUBE)) % 2 == 0)
				color = COLOR_WHITE;
			else
				color = COLOR_BLACK;
			for(int i = 0; i < CUBE; i++)
			{
				for(int j = 0; j < CUBE; j++)
					mlx_put_pixel(background, x + i, y + j, color);
			}
			y = y + CUBE;
		}
		x = x + CUBE;
	}
	mlx_image_to_window(mlx, background, 0, 0);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return 0;
}