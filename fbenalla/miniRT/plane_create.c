/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:22:56 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:31:22 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	coor_plane(t_plane_fb *new_plane, char **data, t_scene *scene)
{
	char	**coors;

	new_plane->coor_plane = malloc(sizeof(t_vec3));
	coors = ft_split(data[1], ',');
	new_plane->coor_plane->x = ft_atoi_double(coors[0], scene->cleaner);
	new_plane->coor_plane->y = ft_atoi_double(coors[1], scene->cleaner);
	new_plane->coor_plane->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "pl", scene->cleaner);
	ft_free_split(coors);
}

void	vects_plane(t_plane_fb *new_plane, char **data, t_scene *scene)
{
	char		**vects;
	double		range_x;
	double		range_y;
	double		range_z;

	new_plane->vector_plane = malloc(sizeof(t_vec3));
	vects = ft_split(data[2], ',');
	new_plane->vector_plane->x = ft_atoi_double(vects[0], scene->cleaner);
	new_plane->vector_plane->y = ft_atoi_double(vects[1], scene->cleaner);
	new_plane->vector_plane->z = ft_atoi_double(vects[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in vectors", "pl", scene->cleaner);
	range_x = new_plane->vector_plane->x;
	range_y = new_plane->vector_plane->y;
	range_z = new_plane->vector_plane->z;
	if (range_x < -1 || range_x > 1 || range_y < -1 || range_y > 1 \
			|| range_z < -1 || range_z > 1)
		exit_error("invalid range vectors", "PL", scene->cleaner);
	if (count_comma(data[3]) != 2)
		exit_error("more or less in color", "in plane", scene->cleaner);
	ft_free_split(vects);
}

void	color_plane(t_plane_fb *new_plane, char **data, t_scene *scene)
{
	char	**colors;

	new_plane->color_plane = malloc(sizeof(t_color_fb));
	colors = ft_split(data[3], ',');
	new_plane->color_plane->r = ft_atoi_color(colors[0], "plane");
	new_plane->color_plane->g = ft_atoi_color(colors[1], "plane");
	new_plane->color_plane->b = ft_atoi_color(colors[2], "plane");
	if (new_plane->color_plane->r == -1 || new_plane->color_plane->g == -1 \
			||new_plane->color_plane->b == -1)
		exit_error("somthing wrong with colors", "in plane", scene->cleaner);
	ft_free_split(colors);
}

void	ft_plane_fb(char **data, t_scene *scene)
{
	t_plane_fb	*new_plane;
	int			i;

	i = 0;
	while (data[i] != NULL)
		i++;
	i--;
	if (!verify_data_plane(data))
		exit_error("data not in the correct format", \
				"in plane", scene->cleaner);
	scene->type = T_PLAN;
	new_plane = malloc(sizeof(t_plane_fb));
	init_new_plane(new_plane);
	if (i == 4 || i == 5)
		bump_mapping_plane_constractor(new_plane, data, scene, i);
	new_plane->next = NULL;
	coor_plane(new_plane, data, scene);
	vects_plane(new_plane, data, scene);
	color_plane(new_plane, data, scene);
	plane_linked_list(new_plane, scene);
}
