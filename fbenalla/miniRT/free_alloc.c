/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:17:40 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 00:18:08 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool is_coordinate(char *coor)
{
	int	i;

	i = 0;
	if (count_comma(coor) != 2)
	{
		printf("coor = %s\n", coor);
		return (false);
	}
	return (true);
}

bool	verify_data_plane(char **data)
{
	int	i;

	i = 0;
	if (!data[1] || !data[2] || !data[3])
		return (false);
	if (!is_coordinate(data[1]))
		return (false);
	if (!is_coordinate(data[2]))
		return (false);
	if (!verify_color(data[3]))
		return (false);
	while (data[i])
		i++;
	if (i > 6)
	{
		printf("alot of arguments in plane\n");
		return (false);
	}
	return (true);
}

bool	verify_data_sphere(char **data, t_cleanup *clean)
{
	int		i;
	double	raduis;

	i = 0;
	if (!data[1] || !data[2] || !data[3])
		return (false);
	if (!is_coordinate(data[1]))
		return (false);

	raduis = ft_atoi_double(data[2], clean);
	if (clean->flag_exit)
		return (false);
	if (!verify_color(data[3]))
		return (false);
	while (data[i])
		i++;
	if (i > 6)
	{
		printf("alot of arguments in sphere\n");
		return (false);
	}
	return (true);
}

bool	verify_data_cylinder(char **data, t_cleanup *clean)
{
	int		i;
	double	height;
	double	diameter;
	int		check;

	i = 0;
	if (!data[1] || !data[2] || !data[3] || !data[4] || !data[5] || !data[6])
		return (false);
	if (!is_coordinate(data[1]))
		return (false);
	if (!is_coordinate(data[2]))
		return (false);
	height = ft_atoi_double(data[3], clean);
	diameter = ft_atoi_double(data[4], clean);
	check = ft_atoi_double(data[5], clean);
	if (clean->flag_exit)
		return (false);
	if (!verify_color(data[6]))
		return (false);
	while (data[i])
		i++;
	if (i > 9)
		return (false);
	return (true);
}

bool	verify_data_cone(char **data, t_cleanup *clean)
{
	int		i;
	double	min;
	double	max;
	double	angle;
	int		flag;

	i = 0;
	if (!data[1] || !data[2] || !data[3] || !data[4] || \
			!data[5] || !data[6] || !data[7])
		return (false);
	if (!is_coordinate(data[1]))
		return (false);
	if (!is_coordinate(data[2]))
		return (false);
	min = ft_atoi_double(data[3], clean);
	max = ft_atoi_double(data[4], clean);
	angle = ft_atoi_double(data[5], clean);
	flag = ft_atoi_double(data[6], clean);
	if (clean->flag_exit)
		return (false);
	if (!verify_color(data[7]))
		return (false);
	while (data[i])
		i++;
	if (i > 10)
		return (false);
	return (true);
}


void	exit_error(char *str, char *scene_name, t_cleanup *cleanup)
{
	int	i;

	i = 0;
	printf("Error\n%s in %s\n", str, scene_name);
	if (cleanup->tokens)
	{
		while (i < cleanup->token_count)
		{
			if (cleanup->tokens[i])
				ft_free_split(cleanup->tokens[i]);
			i++;
		}
		free(cleanup->tokens);
		cleanup->tokens = NULL;
	}
	if (cleanup->dispatched_table && *cleanup->dispatched_table)
	{
		ft_lstclear((void **)cleanup->dispatched_table, del, 'o');
		*cleanup->dispatched_table = NULL;
	}
	if (cleanup->scene)
	{
		free_scene(cleanup->scene);
		cleanup->scene = NULL;
	}
	if (cleanup->input_data)
	{
		free(cleanup->input_data);
		cleanup->input_data = NULL;
	}
	if (cleanup->container)
	{
		ft_lstclear((void **)&cleanup->container, del, 'c');
		cleanup->container = NULL;
	}
	free(cleanup);
	exit(1);
}

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


void	free_sphere(t_scene *scene)
{
	t_sphere_fb			*sp;
	t_sphere_fb			*next_sp;

	sp = scene->sphere;
	while (sp)
	{
		next_sp = sp->next;
		if (sp->flag_bump && sp->bump_texture)
		{
			if (sp->bump_texture->image)
			{
				if (sp->bump_texture->type == 2)
					mlx_delete_texture(sp->bump_texture->image);
				sp->bump_texture->image = NULL;
			}
			if (sp->img_path && sp->bump_texture->type == 2)
			{
				free(sp->img_path);
				sp->img_path = NULL;
			}
			free(sp->bump_texture);
			sp->bump_texture = NULL;
		}
		free(sp->color_sphere);
		free(sp->coor_sphere);
		free(sp);
		sp = next_sp;
	}
}

void	free_plane(t_scene *scene)
{
	t_plane_fb			*pl;
	t_plane_fb			*next_pl;

	pl = scene->plane;
	while (pl)
	{
		next_pl = pl->next;
		if (pl->flag_bump && pl->bump_texture)
		{
			if (pl->bump_texture->image && pl->bump_texture->type == 2)
			{
				mlx_delete_texture(pl->bump_texture->image);
				pl->bump_texture->image = NULL;
			}
			if (pl->img_path && pl->bump_texture->type == 2)
			{
				free(pl->img_path);
				pl->img_path = NULL;
			}
			free(pl->bump_texture);
			pl->bump_texture = NULL;
		}
		free(pl->color_plane);
		free(pl->coor_plane);
		free(pl->vector_plane);
		free(pl);
		pl = next_pl;
	}
}

void	free_cylinder(t_scene *scene)
{
	t_cylinder_fb		*cy;
	t_cylinder_fb		*next_cy;

	cy = scene->cylinder;
	while (cy)
	{
		next_cy = cy->next;
		if (cy->flag_bump && cy->bump_texture)
		{
			if (cy->bump_texture->image && cy->bump_texture->type == 2)
			{
				mlx_delete_texture(cy->bump_texture->image);
				cy->bump_texture->image = NULL;
			}
			if (cy->img_path && cy->bump_texture->type == 2)
			{
				free(cy->img_path);
				cy->img_path = NULL;
			}
			free(cy->bump_texture);
			cy->bump_texture = NULL;
		}
		free(cy->color_cylinder);
		free(cy->coor_cylinder);
		free(cy->vector_cylinder);
		free(cy);
		cy = next_cy;
	}
}

void	free_cone(t_scene *scene)
{
	t_cone_fb		*co;
	t_cone_fb		*next_co;

	co = scene->cone;
	while (co)
	{
		next_co = co->next;
		if (co->flag_bump && co->bump_texture)
		{
			puts("freeing cone");
			if (co->bump_texture->image && co->bump_texture->type == 2)
			{
				mlx_delete_texture(co->bump_texture->image);
				co->bump_texture->image = NULL;
			}
			if (co->img_path && co->bump_texture->type == 2)
			{
				free(co->img_path);
				co->img_path = NULL;
			}
			free(co->bump_texture);
			co->bump_texture = NULL;
		}
		free(co->color_cone);
		free(co->coor_cone);
		free(co->vector_cone);
		free(co);
		co = next_co;
	}
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
	if (scene->ambient_light)
	{
		free(scene->ambient_light->color_ambient_light);
		free(scene->ambient_light);
	}
	if (scene->camera)
	{
		free(scene->camera->coor_camera);
		free(scene->camera->vector_camera);
		free(scene->camera);
	}
	free_light(scene);
	free_sphere(scene);
	free_plane(scene);
	free_cylinder(scene);
	free_cone(scene);
	free(scene);
}
