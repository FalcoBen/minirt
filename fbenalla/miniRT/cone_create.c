/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:07:39 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/13 22:12:10 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	cone_linked_list(t_cone_fb *new_cone, t_scene *scene)
{
	t_cone_fb	*current;

	if (scene->cone == NULL)
	{
		scene->cone = new_cone;
	}
	else
	{
		current = scene->cone;
		while (current->next)
			current = current->next;
		current->next = new_cone;
	}
}

t_texture	*create_checker_bump_cone(void)
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

t_texture	*create_image_bump_cone(char *img_path, t_scene *scene)
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

void	coor_cone(t_cone_fb *new_cone, char **data, t_scene *scene)
{
	char	**coors;

	new_cone->coor_cone = malloc(sizeof(t_vec3));
	coors = ft_split(data[1], ',');
	new_cone->coor_cone->x = ft_atoi_double(coors[0], scene->cleaner);
	new_cone->coor_cone->y = ft_atoi_double(coors[1], scene->cleaner);
	new_cone->coor_cone->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "cone", scene->cleaner);
	ft_free_split(coors);
}

void	vects_cone(t_cone_fb *new_cone, char **data, t_scene *scene)
{
	char	**vects;
	double	range_x;
	double	range_y;
	double	range_z;

	new_cone->vector_cone = malloc(sizeof(t_vec3));
	vects = ft_split(data[2], ',');
	new_cone->vector_cone->x = ft_atoi_double(vects[0], scene->cleaner);
	new_cone->vector_cone->y = ft_atoi_double(vects[1], scene->cleaner);
	new_cone->vector_cone->z = ft_atoi_double(vects[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in vectors", "cone", scene->cleaner);
	range_x = new_cone->vector_cone->x;
	range_y = new_cone->vector_cone->y;
	range_z = new_cone->vector_cone->z;
	if (range_x < -1 || range_x > 1 || range_y < -1 || \
			range_y > 1 || range_z < -1 || range_z > 1)
		exit_error("invalid range vectors", "cone", scene->cleaner);
	ft_free_split(vects);
}

void	color_cone(t_cone_fb *new_cone, char **data, t_scene *scene)
{
	char	**colors;

	new_cone->color_cone = malloc(sizeof(t_color_fb));
	colors = ft_split(data[7], ',');
	new_cone->color_cone->r = ft_atoi_color(colors[0], "cone");
	new_cone->color_cone->g = ft_atoi_color(colors[1], "cone");
	new_cone->color_cone->b = ft_atoi_color(colors[2], "cone");
	ft_free_split(colors);
}

void    init_new_cone(t_cone_fb *new_cone)
{
	new_cone->flag_bump = false;
	new_cone->bump_texture = NULL;
	new_cone->img_path = NULL;
	new_cone->closed_flag = false;
	new_cone->next = NULL;
}

void	other_params_check_cone(t_cone_fb *new_cone, char **data,t_scene *scene)
{
	int	open_or_close;

	new_cone->minimum = ft_atoi_double(data[3], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in minimum", "cone", scene->cleaner);
	new_cone->maximum = ft_atoi_double(data[4], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in hight cone", "cone", scene->cleaner);
	new_cone->angle = ft_atoi_double(data[5], scene->cleaner);
	if (new_cone->angle < 0 || new_cone->angle > 180)
		exit_error("invalid range inangle", "cone", scene->cleaner);
	open_or_close = atoi(data[6]);
	if (open_or_close == 0)
		new_cone->closed_flag = true;
	else if (open_or_close == -1)
		new_cone->closed_flag = false;
	else
		exit_error("flag for closed or open required to be 0 or -1", \
				"in Cylinder", scene->cleaner);
}

void	ft_cone_fb(char **data, t_scene *scene)
{
	t_cone_fb	*new_cone;
	int			i;

	if (!verify_data_cone(data, scene->cleaner))
		exit_error("data not in the correct format", "in cone", scene->cleaner);
	i = 0;
	while (data[i])
		i++;
	i--;
	new_cone = malloc(sizeof(t_cone_fb));
	init_new_cone(new_cone);
	scene->type = T_CONE;
	if (i == 8 || i == 9)
		bump_mapping_cone_constractor(new_cone, data, scene, i);
	coor_cone(new_cone, data, scene);
	vects_cone(new_cone, data, scene);
	other_params_check_cone(new_cone, data, scene);
	color_cone(new_cone, data, scene);
	cone_linked_list(new_cone, scene);
}
