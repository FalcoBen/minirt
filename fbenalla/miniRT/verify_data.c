/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:23:22 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:36:23 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

void	check_valid_args(char **data, t_scene *scene, char object)
{
	int	i;

	i = 0;
	while (data[i])
		i++;
	if (object == 'A')
	{
		if (i != 3)
			exit_error("invalid arguments", "in A", scene->cleaner);
	}
	if (object == 'C')
	{
		if (i != 4)
			exit_error("invalid arguments", "in C", scene->cleaner);
	}
	if (object == 'L')
	{
		if (i != 4)
			exit_error("invalid arguments", "in L", scene->cleaner);
	}
}

bool	verify_data_ambient_light(char **data, t_cleanup *clean)
{
	int		i;
	double	ambient;

	i = 0;
	if (!data[1] || !data[2])
		return (false);
	ambient = ft_atoi_double(data[1], clean);
	if (clean->flag_exit)
		return (false);
	if (!verify_color(data[2]))
		return (false);
	while (data[i])
		i++;
	if (i > 3)
	{
		printf("alot of arguments in ambient_light\n");
		return (false);
	}
	return (true);
}

char	*ft_strdup(const char *s1)
{
	size_t	l;
	size_t	i;
	char	*dest;

	if (!s1)
		return (NULL);
	l = ft_strlen(s1);
	dest = (char *)alloc((l + 1) * sizeof(char), false);
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[l] = '\0';
	return (dest);
}
