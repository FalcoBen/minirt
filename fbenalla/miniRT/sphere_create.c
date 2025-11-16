/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:26:31 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:31:56 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

void	coor_sphere(t_sphere_fb *new_sphere, char **data, t_scene *scene)
{
	char	**coors;

	new_sphere->coor_sphere = alloc(sizeof(t_texture_color), false);
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

	new_sphere->color_sphere = alloc(sizeof(t_color_fb), false);
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

void	ft_sphere_fb(char **data, t_scene *scene)
{
	t_sphere_fb	*new_sphere;
	int			i;

	if (!verify_data_sphere(data, scene->cleaner))
		exit_error("data not in the correct format", \
				"in sphere", scene->cleaner);
	new_sphere = alloc(sizeof(t_sphere_fb), false);
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
