/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:26:11 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:37:14 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	camera_linked_list(t_camera_fb *new_camera, t_scene *scene)
{
	t_camera_fb	*current;
	t_camera_fb	*tmp;
	int			counter;

	if (scene->camera == NULL)
		scene->camera = new_camera;
	else
	{
		current = scene->camera;
		while (current->next)
			current = current->next;
		current->next = new_camera;
	}
	tmp = scene->camera;
	counter = 0;
	while (tmp)
	{
		counter++;
		tmp = tmp->next;
	}
	if (counter > 1)
		exit_error("duplicate object", "camera", scene->cleaner);
}

void	coor_camera(t_camera_fb *new_camera, char **data, t_scene *scene)
{
	char	**coors;

	new_camera->coor_camera = malloc(sizeof(t_vec3));
	coors = ft_split(data[1], ',');
	new_camera->coor_camera->x = ft_atoi_double(coors[0], scene->cleaner);
	new_camera->coor_camera->y = ft_atoi_double(coors[1], scene->cleaner);
	new_camera->coor_camera->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coors", "C", scene->cleaner);
	ft_free_split(coors);
}

void	vector_camera(t_camera_fb *new_camera, char **data, t_scene *scene)
{
	char	**vects;
	double	range_x;
	double	range_y;
	double	range_z;

	new_camera->vector_camera = malloc(sizeof(t_vec3));
	vects = ft_split(data[2], ',');
	new_camera->vector_camera->x = ft_atoi_double(vects[0], scene->cleaner);
	new_camera->vector_camera->y = ft_atoi_double(vects[1], scene->cleaner);
	new_camera->vector_camera->z = ft_atoi_double(vects[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in vectors", "C", scene->cleaner);
	range_x = new_camera->vector_camera->x;
	range_y = new_camera->vector_camera->y;
	range_z = new_camera->vector_camera->z;
	if (range_x < -1 || range_x > 1)
		exit_error("invalid range vectors", "C in x", scene->cleaner);
	if (range_y < -1 || range_y > 1)
		exit_error("invalid range vectors", "C in y", scene->cleaner);
	if (range_z < -1 || range_z > 1)
		exit_error("invalid range vectors", "C in z", scene->cleaner);
	ft_free_split(vects);
}

bool	verify_data_camera(char **data, t_cleanup *clean)
{
	int		i;
	double	angle;

	i = 0;
	if (!data[1] || !data[2] || !data[3])
		return (false);
	if (!is_coordinate(data[1]))
		return (false);
	if (!is_coordinate(data[2]))
		return (false);
	angle = ft_atoi_double(data[3], clean);
	if (clean->flag_exit)
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

void	ft_camera_fb(char **data, t_scene *scene)
{
	t_camera_fb	*new_camera;
	t_camera_fb	*current;

	check_valid_args(data, scene, 'C');
	if (!verify_data_camera(data, scene->cleaner))
		exit_error("data not in the correct format", \
				"in camera", scene->cleaner);
	new_camera = malloc(sizeof(t_camera_fb));
	new_camera->next = NULL;
	coor_camera(new_camera, data, scene);
	vector_camera(new_camera, data, scene);
	new_camera->angle_view = ft_atoi_double(data[3], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in angle", "C", scene->cleaner);
	if (new_camera->angle_view < 0 || new_camera->angle_view > 180)
		exit_error("invalid range angle view", "C", scene->cleaner);
	camera_linked_list(new_camera, scene);
}
