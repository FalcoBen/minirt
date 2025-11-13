/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_create.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:12:31 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/13 22:16:43 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	cylinder_linked_list(t_cylinder_fb *new_cylinder, t_scene *scene)
{
	t_cylinder_fb	*current;

	if (scene->cylinder == NULL)
		scene->cylinder = new_cylinder;
	else
	{
		current = scene->cylinder;
		while (current->next)
			current = current->next;
		current->next = new_cylinder;
	}
}

t_texture	*create_checker_bump_cylinder(void)
{
	t_texture	*test_bump_checker;

	test_bump_checker = malloc(sizeof(t_texture));
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
		free(new_cylinder);
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
		free(new_cylinder);
		exit_error("malformat in type of bump", "in cylinder", scene->cleaner);
	}
	new_cylinder->flag_bump = true;
	if (type_bump == 1)
		bump_mapping_cylinder_type_1(new_cylinder, i, scene);
	else
		bump_mapping_cylinder_type_2(new_cylinder, data, i, scene);
}

void	coor_cylinder(t_cylinder_fb *new_cylinder, char **data, t_scene *scene)
{
	char	**coors;

	new_cylinder->coor_cylinder = malloc(sizeof(t_vec3));
	coors = ft_split(data[1], ',');
	new_cylinder->coor_cylinder->x = ft_atoi_double(coors[0], scene->cleaner);
	new_cylinder->coor_cylinder->y = ft_atoi_double(coors[1], scene->cleaner);
	new_cylinder->coor_cylinder->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "cy", scene->cleaner);
	ft_free_split(coors);
}

void	vects_cylinder(t_cylinder_fb *new_cylinder, char **data, t_scene *scene)
{
	char	**vects;
	double	range_x;
	double	range_y;
	double	range_z;

	new_cylinder->vector_cylinder = malloc(sizeof(t_vec3));
	vects = ft_split(data[2], ',');
	new_cylinder->vector_cylinder->x = ft_atoi_double(vects[0], scene->cleaner);
	new_cylinder->vector_cylinder->y = ft_atoi_double(vects[1], scene->cleaner);
	new_cylinder->vector_cylinder->z = ft_atoi_double(vects[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in vectors", "cy", scene->cleaner);
	range_x = new_cylinder->vector_cylinder->x;
	range_y = new_cylinder->vector_cylinder->y;
	range_z = new_cylinder->vector_cylinder->z;
	if (range_x < -1 || range_x > 1 || range_y < -1 || \
			range_y > 1 || range_z < -1 || range_z > 1)
		exit_error("invalid range vectors", "CY", scene->cleaner);
	ft_free_split(vects);
}

void	color_cylinder(t_cylinder_fb *new_cylinder, char **data, t_scene *scene)
{
	char	**colors;

	new_cylinder->color_cylinder = malloc(sizeof(t_color_fb));
	colors = ft_split(data[6], ',');
	new_cylinder->color_cylinder->r = ft_atoi_color(colors[0], "cylinder");
	new_cylinder->color_cylinder->g = ft_atoi_color(colors[1], "cylinder");
	new_cylinder->color_cylinder->b = ft_atoi_color(colors[2], "cylinder");
	ft_free_split(colors);
}

void	init_new_cylinder(t_cylinder_fb *new_cylinder)
{
	new_cylinder->flag_bump = false;
	new_cylinder->closed = false;
	new_cylinder->bump_texture = NULL;
	new_cylinder->img_path = NULL;
	new_cylinder->next = NULL;
}

void	other_paranms_check(t_cylinder_fb *new_cylinder, \
		char **data, t_scene *scene)
{
	int	open_or_close;

	new_cylinder->diameter_cylinder = ft_atoi_double(data[3], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in diameter_cylinder", "cy", scene->cleaner);
	new_cylinder->height_cylinder_fb = ft_atoi_double(data[4], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in hight cylinder", "cy", scene->cleaner);
	if (new_cylinder->height_cylinder_fb <= 0 || \
			new_cylinder->diameter_cylinder <= 0)
		exit_error("invalid range in height or diameter", "CY", scene->cleaner);
	open_or_close = atoi(data[5]);
	if (open_or_close == 0)
		new_cylinder->closed = true;
	else if (open_or_close == -1)
		new_cylinder->closed = false;
	else
		exit_error("flag for closed or open required to be 0 or -1", \
				"in Cylinder", scene->cleaner);
}

void	ft_cylinder_fb(char **data, t_scene *scene)
{
	t_cylinder_fb	*new_cylinder;
	int				i;

	if (!verify_data_cylinder(data, scene->cleaner))
		exit_error("data not in the correct format", \
				"in cylinder", scene->cleaner);
	i = 0;
	while (data[i])
		i++;
	i--;
	new_cylinder = malloc(sizeof(t_cylinder_fb));
	init_new_cylinder(new_cylinder);
	scene->type = T_CYLINDRE;
	if (i == 7 || i == 8)
		bump_mapping_cylinder_constractor(new_cylinder, data, scene, i);
	coor_cylinder(new_cylinder, data, scene);
	vects_cylinder(new_cylinder, data, scene);
	other_paranms_check(new_cylinder, data, scene);
	color_cylinder(new_cylinder, data, scene);
	cylinder_linked_list(new_cylinder, scene);
}
