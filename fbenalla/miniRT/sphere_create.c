/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:26:31 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 00:03:13 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	sphere_linked_list(t_sphere_fb *new_sphere, t_scene *scene)
{
	t_sphere_fb	*current;

	if (scene->sphere == NULL)
	{
		puts("in if spher\n");	
		scene->sphere = new_sphere;
	}
	else
	{
		current = scene->sphere;
		while (current->next)
			current = current->next;
		current->next = new_sphere;
	}
}

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

void	bump_mapping_sphere_type_1(t_sphere_fb *new_sphere, int i, t_scene *scene)
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

/*--------------------------------------*/

void	coor_sphere(t_sphere_fb *new_sphere, char **data, t_scene *scene)
{
	char	**coors;

	new_sphere->coor_sphere = malloc(sizeof(t_vec3));
	coors = ft_split(data[1], ',');
	new_sphere->coor_sphere->x = ft_atoi_double(coors[0], scene->cleaner);
	new_sphere->coor_sphere->y = ft_atoi_double(coors[1], scene->cleaner);
	new_sphere->coor_sphere->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "sp", scene->cleaner);
	ft_free_split(coors);
}

void	color_sphere(t_sphere_fb *new_sphere, char **data, t_scene *scene)
{
	char	**colors;

	new_sphere->color_sphere = malloc(sizeof(t_color_fb));
	if (count_comma(data[3]) != 2)
		exit_error("more or less in color", "in spher", scene->cleaner);
	colors = ft_split(data[3], ',');
	new_sphere->color_sphere->r = ft_atoi_color(colors[0], "sphere");
	new_sphere->color_sphere->g = ft_atoi_color(colors[1], "sphere");
	new_sphere->color_sphere->b = ft_atoi_color(colors[2], "sphere");
	if (new_sphere->color_sphere->r == -1 || new_sphere->color_sphere->g == -1 \
			|| new_sphere->color_sphere->b == -1)
		exit_error("somthing wrong with colors", "in sphere", scene->cleaner);
	ft_free_split(colors);
}

void     init_new_sphere(t_sphere_fb *new_sphere)
{
	new_sphere->next = NULL;
	new_sphere->bump_texture = NULL;
	new_sphere->img_path = NULL;
	new_sphere->flag_bump = false;
	new_sphere->type = 0;
}

void	ft_sphere_fb(char **data, t_scene *scene)
{
	t_sphere_fb	*new_sphere;
	int			i;

	if (!verify_data_sphere(data, scene->cleaner))
		exit_error("data not in the correct format", \
				"in sphere", scene->cleaner);
	new_sphere = malloc(sizeof(t_sphere_fb));
	init_new_sphere(new_sphere);
	i = 0;
	while (data[i] != NULL)
		i++;
	i--;
	scene->type = T_SPHERE;
	if (i == 4 || i == 5)
		bump_mapping_sphere_constractor(new_sphere, data, scene, i);
	coor_sphere(new_sphere, data, scene);
	new_sphere->diameter_sphere = ft_atoi_double(data[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in diameter", "sp", scene->cleaner);
	if (new_sphere->diameter_sphere <= 0)
		exit_error("invalid diametre", "in sphere", scene->cleaner);
	color_sphere(new_sphere, data, scene);
	sphere_linked_list(new_sphere, scene);
}
