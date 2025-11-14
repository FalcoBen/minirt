/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:53:25 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:31:07 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	objects_linking_constractor(char *identifier, \
		void (*assign_object)(char **, t_scene *), \
			t_objects_fb **head, t_objects_fb *current)
{
	t_objects_fb	*new_node;

	new_node = malloc(sizeof(t_objects_fb));
	if (!new_node)
		return ;
	new_node->identifier = ft_strdup(identifier);
	new_node->assign_object = assign_object;
	new_node->next = NULL;
	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	add_object(t_objects_fb **head, char *identifier, \
		void (*assign_object)(char **, t_scene *))
{
	t_objects_fb	*current;

	current = *head;
	while (current)
	{
		if (current->identifier && strcmp(current->identifier, identifier) == 0)
		{
			return ;
		}
		current = current->next;
	}
	objects_linking_constractor(identifier, assign_object, head, current);
}

void	init_object_dispatch_table(t_objects_fb **dispatch_table)
{
	add_object(dispatch_table, "A", ft_ambient_light_fb);
	add_object(dispatch_table, "C", ft_camera_fb);
	add_object(dispatch_table, "L", ft_light);
	add_object(dispatch_table, "pl", ft_plane_fb);
	add_object(dispatch_table, "sp", ft_sphere_fb);
	add_object(dispatch_table, "cy", ft_cylinder_fb);
	add_object(dispatch_table, "cone", ft_cone_fb);
}
