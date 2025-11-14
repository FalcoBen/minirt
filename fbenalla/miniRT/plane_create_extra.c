/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_create_extra.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:54:03 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 00:55:09 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	plane_linked_list(t_plane_fb *new_plane, t_scene *scene)
{
	t_plane_fb	*current;

	if (scene->plane == NULL)
		scene->plane = new_plane;
	else
	{
		current = scene->plane;
		while (current->next)
			current = current->next;
		current->next = new_plane;
	}
}

void	init_new_plane(t_plane_fb *new_plane)
{
	new_plane->flag_bump = false;
	new_plane->bump_texture = NULL;
	new_plane->img_path = NULL;
	new_plane->next = NULL;
}
