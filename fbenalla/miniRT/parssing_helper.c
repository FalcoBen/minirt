/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:13:52 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:20:52 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

t_container	*ft_lstlast(t_container *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

bool	is_coordinate(char *coor)
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

void	free_tokens(t_cleanup *cleanup)
{
	int	i;

	i = 0;
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
}

void	exit_error(char *str, char *scene_name, t_cleanup *cleanup)
{
	printf("Error\n%s in %s\n", str, scene_name);
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
	alloc(0, true);
	exit(1);
}
