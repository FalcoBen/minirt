/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:21:23 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/13 23:22:38 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	check_extension(char *str)
{
	char	*extension;
	char	*input_extension;

	input_extension = ft_strrchr(str, '.');
	extension = ".rt";
	if (!input_extension)
		return (false);
	if (strncmp(input_extension, extension, ft_strlen(input_extension)) == 0)
		return (true);
	return (false);
}

void	initialize_scenes(t_scene *scene)
{
	scene->ambient_light = NULL;
	scene->camera = NULL;
	scene->light = NULL;
	scene->plane = NULL;
	scene->sphere = NULL;
	scene->cylinder = NULL;
	scene->cone = NULL;
}

int	main(int ac, char **av)
{
	char	*file_name;
	int		fd;

	if (ac != 2)
		return (1);
	file_name = av[1];
	if (!check_extension(file_name))
	{
		printf("Error\n, bad extension\n");
		return (1);
	}
	fd = open(file_name, O_RDONLY, 0644);
	if (fd < 0)
	{
		printf("failed to open file\n");
		return (2);
	}
	parsing(fd);
	return (0);
}
