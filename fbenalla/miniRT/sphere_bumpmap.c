/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_bumpmap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:56:24 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:35:21 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_texture	*create_checker_bump_sphere(void)
{
	t_texture	*test_bump_checker;

	test_bump_checker = malloc(sizeof(t_texture));
	if (!test_bump_checker)
		return (NULL);
	test_bump_checker->type = 1;
	test_bump_checker->color_solid = (t_color_fb){0, 0, 0};
	test_bump_checker->color_checkerd = (t_color_fb){255, 255, 255};
	test_bump_checker->scale = 0.1;
	test_bump_checker->image = NULL;
	return (test_bump_checker);
}

t_texture	*create_image_bump_sphere(char *img_path, t_scene *scene)
{
	t_texture	*test_bump;

	test_bump = malloc(sizeof(t_texture));
	if (!test_bump)
		return (NULL);
	test_bump->type = 2;
	test_bump->color_solid = (t_color_fb){0, 0, 0};
	test_bump->color_checkerd = (t_color_fb){0, 0, 0};
	test_bump->scale = 0.1;
	test_bump->image = mlx_load_png(img_path);
	if (!test_bump->image)
	{
		free(img_path);
		free(test_bump);
		perror("png");
		exit_error("incorrect image path", "in sphere", scene->cleaner);
	}
	return (test_bump);
}

void	bump_mapping_sphere_type_1(t_sphere_fb *new_sphere, \
		int i, t_scene *scene)
{
	if (i == 5)
		exit_error("nothing should be after this type of bump", \
				"in sphere", scene->cleaner);
	new_sphere->img_path = NULL;
	new_sphere->bump_texture = create_checker_bump_sphere();
}

void	bump_mapping_sphere_type_2(t_sphere_fb *new_sphere, \
		char **data, int i, t_scene *scene)
{
	if (i != 5)
	{
		free(new_sphere);
		exit_error("need a png", "in sphere", scene->cleaner);
	}
	new_sphere->img_path = ft_strdup(data[5]);
	new_sphere->bump_texture = \
		create_image_bump_sphere(new_sphere->img_path, scene);
}

void	bump_mapping_sphere_constractor(t_sphere_fb *new_sphere, \
		char **data, t_scene *scene, int i)
{
	int	type_bump;

	type_bump = ft_atoi_color(data[4], "bump in sphere");
	if (type_bump != 1 && type_bump != 2)
	{
		free(new_sphere);
		exit_error("malformat in type of bump", "in sphere", scene->cleaner);
	}
	new_sphere->flag_bump = true;
	if (type_bump == 1)
		bump_mapping_sphere_type_1(new_sphere, i, scene);
	else
		bump_mapping_sphere_type_2(new_sphere, data, i, scene);
}
