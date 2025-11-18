/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan_bumpmap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:54:02 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 00:54:26 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

t_texture	*create_checker_bump(void)
{
	t_texture	*test_bump_checker;

	test_bump_checker = alloc(sizeof(t_texture), false);
	test_bump_checker->type = 1;
	test_bump_checker->scale = 0.1;
	test_bump_checker->image = NULL;
	return (test_bump_checker);
}

t_texture	*create_image_bump(char *img_path, t_scene *scene)
{
	t_texture	*test_bump;

	test_bump = alloc(sizeof(t_texture), false);
	test_bump->type = 2;
	test_bump->color_solid = (t_color_fb){0, 0, 0};
	test_bump->color_checkerd = (t_color_fb){0, 0, 0};
	test_bump->scale = 0.002;
	test_bump->image = mlx_load_png(img_path);
	if (!test_bump->image)
	{
		perror("png");
		exit_error("incorrect image path", "in plane", scene->cleaner);
	}
	return (test_bump);
}

void	bump_mapping_plane_type_1(t_plane_fb *new_plane, int i, t_scene *scene)
{
	if (i == 5)
		exit_error("nothing should be after this type of bump", \
				"in plane", scene->cleaner);
	new_plane->img_path = NULL;
	new_plane->bump_texture = create_checker_bump();
}

void	bump_mapping_plane_type_2(t_plane_fb *new_plane, \
		char **data, int i, t_scene *scene)
{
	if (i != 5)
	{
		exit_error("need a png", "in plane", scene->cleaner);
	}
	new_plane->img_path = ft_strdup(data[5]);
	new_plane->bump_texture = create_image_bump(new_plane->img_path, scene);
}

void	bump_mapping_plane_constractor(t_plane_fb *new_plane, \
		char **data, t_scene *scene, int i)
{
	int	type_bump;

	type_bump = ft_atoi_color(data[4], "bump plane");
	if (type_bump != 1 && type_bump != 2)
	{
		exit_error("malformat in type of bump ", "in plane", scene->cleaner);
	}
	new_plane->flag_bump = true;
	if (type_bump == 1)
		bump_mapping_plane_type_1(new_plane, i, scene);
	else
		bump_mapping_plane_type_2(new_plane, data, i, scene);
}
