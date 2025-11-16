/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:17:40 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:14:33 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	del(void *content)
{
	free(content);
}

void	ft_free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_light(t_scene *scene)
{
	t_light_fb	*light;
	t_light_fb	*next_light;

	light = scene->light;
	while (light)
	{
		next_light = light->next;
		free(light->color_light);
		free(light->coor_light);
		free(light);
		light = next_light;
	}
}

void	free_scene(t_scene *scene)
{
	free_sphere(scene);
	free_plane(scene);
	free_cylinder(scene);
	free_cone(scene);
}
