/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_bumpmap.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:51:46 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:35:09 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

t_texture	*create_checker_bump_cylinder(void)
{
	t_texture	*test_bump_checker;

	test_bump_checker = alloc(sizeof(t_texture), false);
	if (!test_bump_checker)
		return (NULL);
	test_bump_checker->type = 1;
	test_bump_checker->scale = 0.1;
	test_bump_checker->image = NULL;
	return (test_bump_checker);
}

t_texture	*create_image_bump_cylinder(char *img_path, t_scene *scene)
{
	t_texture	*test_bump;

	test_bump = alloc(sizeof(t_texture), false);
	if (!test_bump)
		return (NULL);
	test_bump->type = 2;
	test_bump->color_solid = (t_color_fb){0, 0, 0};
	test_bump->color_checkerd = (t_color_fb){0, 0, 0};
	test_bump->scale = 0.1;
	test_bump->image = mlx_load_png(img_path);
	if (!test_bump->image)
	{
		perror("png");
		exit_error("incorrect image path", "in cylinder", scene->cleaner);
	}
	return (test_bump);
}

void	bump_mapping_cylinder_type_1(t_cylinder_fb *new_cylinder, \
		int i, t_scene *scene)
{
	if (i != 7)
		exit_error("nothing should be after this type of bump", \
				"in cylinder", scene->cleaner);
	new_cylinder->img_path = NULL;
	new_cylinder->bump_texture = create_checker_bump_cylinder();
}

void	bump_mapping_cylinder_type_2(t_cylinder_fb *new_cylinder, \
		char **data, int i, t_scene *scene)
{
	if (i != 8)
	{
		exit_error("need a png", "in cylinder", scene->cleaner);
	}
	new_cylinder->img_path = ft_strdup(data[8]);
	new_cylinder->bump_texture = \
		create_image_bump_cylinder(new_cylinder->img_path, scene);
}

void	bump_mapping_cylinder_constractor(t_cylinder_fb *new_cylinder, \
		char **data, t_scene *scene, int i)
{
	int	type_bump;

	type_bump = ft_atoi_color(data[7], "bump in cylinder");
	if (type_bump != 1 && type_bump != 2)
	{
		exit_error("malformat in type of bump", "in cylinder", scene->cleaner);
	}
	new_cylinder->flag_bump = true;
	if (type_bump == 1)
		bump_mapping_cylinder_type_1(new_cylinder, i, scene);
	else
		bump_mapping_cylinder_type_2(new_cylinder, data, i, scene);
}
