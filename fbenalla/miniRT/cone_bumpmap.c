/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_bumpmap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:49:33 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:34:31 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

t_texture	*create_checker_bump_cone(void)
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

t_texture	*create_image_bump_cone(char *img_path, t_scene *scene)
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
		free(img_path);
		free(test_bump);
		perror("png");
		exit_error("incorrect image path", "in cone", scene->cleaner);
	}
	return (test_bump);
}

void	bump_mapping_cone_type_1(t_cone_fb *new_cone, int i, t_scene *scene)
{
	if (i != 8)
		exit_error("nothing should be after this type of bump", \
				"in cone", scene->cleaner);
	new_cone->img_path = NULL;
	new_cone->bump_texture = create_checker_bump_cone();
}

void	bump_mapping_cone_type_2(t_cone_fb *new_cone, char **data, \
		int i, t_scene *scene)
{
	if (i != 9)
	{
		free(new_cone);
		exit_error("need a png", "in cone", scene->cleaner);
	}
	new_cone->img_path = ft_strdup(data[9]);
	new_cone->bump_texture = create_image_bump_cone(new_cone->img_path, scene);
}

void	bump_mapping_cone_constractor(t_cone_fb *new_cone, \
		char **data, t_scene *scene, int i)
{
	int	type_bump;

	type_bump = ft_atoi_color(data[8], "bump in cone");
	if (type_bump != 1 && type_bump != 2)
	{
		free(new_cone);
		exit_error("malformat in type of bump", "in cone", scene->cleaner);
	}
	new_cone->flag_bump = true;
	if (type_bump == 1)
		bump_mapping_cone_type_1(new_cone, i, scene);
	else
		bump_mapping_cone_type_2(new_cone, data, i, scene);
}
