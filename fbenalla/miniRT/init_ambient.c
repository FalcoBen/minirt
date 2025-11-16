/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ambient.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:27:45 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:36:49 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

void	ambient_light_linked_list(t_ambient_light_fb *new_ambient, \
		t_scene *scene)
{
	t_ambient_light_fb	*current;
	t_ambient_light_fb	*tmp;
	int					counter;

	if (scene->ambient_light == NULL)
		scene->ambient_light = new_ambient;
	else
	{
		current = scene->ambient_light;
		while (current->next)
			current = current->next;
		current->next = new_ambient;
	}
	tmp = scene->ambient_light;
	counter = 0;
	while (tmp)
	{
		counter++;
		tmp = tmp->next;
	}
	if (counter > 1)
		exit_error("duplicate object", "ambient_light", scene->cleaner);
}

void	color_ambient(char **data, \
		t_ambient_light_fb *new_ambient, t_scene *scene)
{
	char				**colors;

	colors = ft_split(data[2], ',');
	new_ambient->color_ambient_light->r = \
		ft_atoi_color(colors[0], "ambient light r");
	new_ambient->color_ambient_light->g = \
		ft_atoi_color(colors[1], "ambient light g");
	new_ambient->color_ambient_light->b = \
		ft_atoi_color(colors[2], "ambient light b");
}

void	ft_ambient_light_fb(char **data, t_scene *scene)
{	
	t_ambient_light_fb	*new_ambient;

	check_valid_args(data, scene, 'A');
	if (!verify_data_ambient_light(data, scene->cleaner))
		exit_error("data not in the correct format", \
			"in ambient_light", scene->cleaner);
	new_ambient = alloc(sizeof(t_ambient_light_fb), false);
	new_ambient->color_ambient_light = alloc(sizeof(t_color_fb), false);
	new_ambient->next = NULL;
	new_ambient->bright_ambient_light_fb = \
		ft_atoi_double(data[1], scene->cleaner);
	ambient_light_linked_list(new_ambient, scene);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in", "A", scene->cleaner);
	if (new_ambient->bright_ambient_light_fb < 0 || \
			new_ambient->bright_ambient_light_fb > 1)
		exit_error("invalid range ambient light", "A", scene->cleaner);
	if (count_comma(data[2]) != 2)
		exit_error("more or less in color", "in ambient_light", scene->cleaner);
	color_ambient(data, new_ambient, scene);
}
