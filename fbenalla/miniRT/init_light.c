/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_light.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:26:07 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:36:08 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

void	light_linked_list(t_light_fb *new_light, t_scene *scene)
{
	t_light_fb	*current;

	if (scene->light == NULL)
		scene->light = new_light;
	else
	{
		current = scene->light;
		while (current->next)
			current = current->next;
		current->next = new_light;
	}
}

void	coor_light(t_light_fb *new_light, char **data, t_scene *scene)
{
	char	**coors;

	new_light->coor_light = alloc(sizeof(t_texture_color), false);
	coors = ft_split(data[1], ',');
	new_light->coor_light->x = ft_atoi_double(coors[0], scene->cleaner);
	new_light->coor_light->y = ft_atoi_double(coors[1], scene->cleaner);
	new_light->coor_light->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "L", scene->cleaner);
	new_light->bright_light = ft_atoi_double(data[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in bright_light", "L", scene->cleaner);
	if (new_light->bright_light < 0 || new_light->bright_light > 1)
		exit_error("invalid range bright light", "L", scene->cleaner);
	if (count_comma(data[3]) != 2)
		exit_error("more or less in color", "in light", scene->cleaner);
}

bool	verify_data_light(char **data, t_cleanup *clean)
{
	int		i;
	double	bright;

	i = 0;
	if (!data[1] || !data[2] || !data[3])
		return (false);
	if (!is_coordinate(data[1]))
		return (false);
	bright = ft_atoi_double(data[2], clean);
	if (clean->flag_exit)
		return (false);
	if (!verify_color(data[3]))
		return (false);
	while (data[i])
		i++;
	if (i > 4)
	{
		printf("alot of arguments in light\n");
		return (false);
	}
	return (true);
}

void	ft_light(char **data, t_scene *scene)
{
	t_light_fb		*new_light;
	char			**colors;

	check_valid_args(data, scene, 'L');
	if (!verify_data_light(data, scene->cleaner))
		exit_error("data not in the correct format", \
				"in light", scene->cleaner);
	new_light = alloc(sizeof(t_light_fb), false);
	new_light->next = NULL;
	light_linked_list(new_light, scene);
	coor_light(new_light, data, scene);
	new_light->color_light = alloc(sizeof(t_color_fb), false);
	colors = ft_split(data[3], ',');
	new_light->color_light->r = ft_atoi_color(colors[0], "light");
	new_light->color_light->g = ft_atoi_color(colors[1], "light");
	new_light->color_light->b = ft_atoi_color(colors[2], "light");
	if (new_light->color_light->r == -1 || \
			new_light->color_light->g == -1 || new_light->color_light->b == -1)
		exit_error("somthing wrong with colors", "in light", scene->cleaner);
}
