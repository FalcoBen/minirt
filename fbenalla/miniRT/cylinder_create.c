/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_create.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:12:31 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 00:52:10 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

void	coor_cylinder(t_cylinder_fb *new_cylinder, char **data, t_scene *scene)
{
	char	**coors;

	new_cylinder->coor_cylinder = alloc(sizeof(t_texture_color), false);
	coors = ft_split(data[1], ',');
	new_cylinder->coor_cylinder->x = ft_atoi_double(coors[0], scene->cleaner);
	new_cylinder->coor_cylinder->y = ft_atoi_double(coors[1], scene->cleaner);
	new_cylinder->coor_cylinder->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "cy", scene->cleaner);
}

void	vects_cylinder(t_cylinder_fb *new_cylinder, char **data, t_scene *scene)
{
	char	**vects;
	double	range_x;
	double	range_y;
	double	range_z;

	new_cylinder->vector_cylinder = alloc(sizeof(t_texture_color), false);
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
}

void	color_cylinder(t_cylinder_fb *new_cylinder, char **data, t_scene *scene)
{
	char	**colors;

	new_cylinder->color_cylinder = alloc(sizeof(t_color_fb), false);
	colors = ft_split(data[6], ',');
	new_cylinder->color_cylinder->r = ft_atoi_color(colors[0], "cylinder");
	new_cylinder->color_cylinder->g = ft_atoi_color(colors[1], "cylinder");
	new_cylinder->color_cylinder->b = ft_atoi_color(colors[2], "cylinder");
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
	open_or_close = ft_atoi(data[5]);
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
	new_cylinder = alloc(sizeof(t_cylinder_fb), false);
	init_new_cylinder(new_cylinder);
	scene->type = T_CYLINDRE;
	if (i == 7 || i == 8)
		bump_mapping_cylinder_constractor(new_cylinder, data, scene, i);
	cylinder_linked_list(new_cylinder, scene);
	coor_cylinder(new_cylinder, data, scene);
	vects_cylinder(new_cylinder, data, scene);
	other_paranms_check(new_cylinder, data, scene);
	color_cylinder(new_cylinder, data, scene);
}
