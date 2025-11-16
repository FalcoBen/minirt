/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:07:39 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:34:50 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

void	coor_cone(t_cone_fb *new_cone, char **data, t_scene *scene)
{
	char	**coors;

	new_cone->coor_cone = alloc(sizeof(t_texture_color), false);
	coors = ft_split(data[1], ',');
	new_cone->coor_cone->x = ft_atoi_double(coors[0], scene->cleaner);
	new_cone->coor_cone->y = ft_atoi_double(coors[1], scene->cleaner);
	new_cone->coor_cone->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "cone", scene->cleaner);
}

void	vects_cone(t_cone_fb *new_cone, char **data, t_scene *scene)
{
	char	**vects;
	double	range_x;
	double	range_y;
	double	range_z;

	new_cone->vector_cone = alloc(sizeof(t_texture_color), false);
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
}

void	color_cone(t_cone_fb *new_cone, char **data, t_scene *scene)
{
	char	**colors;

	new_cone->color_cone = alloc(sizeof(t_color_fb), false);
	colors = ft_split(data[7], ',');
	new_cone->color_cone->r = ft_atoi_color(colors[0], "cone");
	new_cone->color_cone->g = ft_atoi_color(colors[1], "cone");
	new_cone->color_cone->b = ft_atoi_color(colors[2], "cone");
}

void	other_params_check_cone(t_cone_fb *new_cone, \
		char **data, t_scene *scene)
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
	open_or_close = ft_atoi(data[6]);
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
	new_cone = alloc(sizeof(t_cone_fb), false);
	init_new_cone(new_cone);
	scene->type = T_CONE;
	if (i == 8 || i == 9)
		bump_mapping_cone_constractor(new_cone, data, scene, i);
	cone_linked_list(new_cone, scene);
	coor_cone(new_cone, data, scene);
	vects_cone(new_cone, data, scene);
	other_params_check_cone(new_cone, data, scene);
	color_cone(new_cone, data, scene);
}
