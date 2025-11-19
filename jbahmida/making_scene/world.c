/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 22:44:39 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/11 22:44:51 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	write_pixel(void *c)
{
	t_nwrite_pixel	var;

	printf("=== Starting render ===\n");
	var.canva = (t_canva *)c;
	var.y = 0;
	while (var.y < var.canva->cam->vsize)
	{
		var.x = 0;
		while (var.x < var.canva->cam->hsize)
		{
			var.ray = s_ray_for_camera(var.canva->cam, var.x, var.y);
			var.color = color_at(var.canva->world, &var.ray);
			var.a = (var.y * WIDTH + var.x) * sizeof(int32_t);
			*((int32_t *)(var.canva->image->pixels
						+ var.a)) = combine_color(var.color);
			var.x++;
		}
		var.y++;
	}
	printf("done\n");
}

t_tuple	get_up_vector(t_tuple *orientation)
{
	t_tuple	world_up;

	world_up = (t_tuple){0, 1, 0, 0};
	if (fabsf(orientation->x) < 0.001 && fabsf(orientation->z) < 0.001)
	{
		return ((t_tuple){0, 0, 1, 0});
	}
	return (world_up);
}

void	close_window(mlx_key_data_t keydata, void *param)
{
	t_canva	*d;

	d = (t_canva *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window(d->mlx);
		printf("Closing window normally.\n");
	}
}

t_camera	*create_camera(t_scene *scene)
{
	t_ncam	cam;

	cam.cam_pos = s_create_tuple((float [3]){scene->camera->coor_camera->x, \
			scene->camera->coor_camera->y, scene->camera->coor_camera->z}, 1);
	cam.cam_dir = s_create_tuple((float [3]){scene->camera->vector_camera->x, \
			scene->camera->vector_camera->y,
			scene->camera->vector_camera->z}, 0);
	cam.length = sqrtl(cam.cam_dir.x * cam.cam_dir.x + cam.cam_dir.y
			* cam.cam_dir.y + cam.cam_dir.z * cam.cam_dir.z);
	if (cam.length > 0.0001)
	{
		cam.cam_dir.x /= cam.length;
		cam.cam_dir.y /= cam.length;
		cam.cam_dir.z /= cam.length;
	}
	cam.to.x = cam.cam_pos.x + cam.cam_dir.x;
	cam.to.y = cam.cam_pos.y + cam.cam_dir.y;
	cam.to.z = cam.cam_pos.z + cam.cam_dir.z;
	cam.to.w = 1;
	cam.up = get_up_vector(&cam.cam_dir);
	cam.fov_radians = (scene->camera->angle_view * M_PI) / 180.0;
	cam.tran = view_transformation(&cam.cam_pos, &cam.to, &cam.up);
	return (camera(WIDTH, HEIGHT, cam.fov_radians, &cam.tran));
}

void	jassim_mlx(t_scene *scene)
{
	mlx_t	*mlx;
	t_canva	canva;
	t_world	w;

	mlx = mlx_init(WIDTH, HEIGHT, "MiniRT", true);
	if (!mlx)
	{
		printf("%s\n", mlx_strerror(mlx_errno));
		return ;
	}
	canva = s_canva_create(mlx, WIDTH, HEIGHT);
	w = s_world(scene);
	canva.world = &w;
	canva.cam = create_camera(scene);
	check_against_null(&canva, scene);
	write_pixel((void *)&canva);
	mlx_image_to_window(canva.mlx, canva.image, 0, 0);
	mlx_key_hook(mlx, &close_window, &canva);
	mlx_loop(mlx);
	clean_textures(scene, &canva, mlx);
	alloc(0, true);
}
