/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 02:03:55 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 02:05:57 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_check(int signe)
{
	if (signe % 2 != 0)
		return (0);
	return (-1);
}

int	ft_atoi(char *str)
{
	int						i;
	int						signe;
	unsigned long long		nb;

	i = 0;
	nb = 0;
	signe = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			signe++;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (nb > (unsigned long long)
			((9223372036854775807 - (str[i] - 48)) / 10))
			return (ft_check(signe));
		nb = nb * 10 + str[i++] - 48;
	}
	if (signe % 2 != 0)
		nb *= -1;
	return ((int)nb);
}

void	make_sure_of_objects(t_scene *scene, t_cleanup *cleaner)
{
	if (!scene->camera)
		exit_error("Missing", "camera", cleaner);
	if (!scene->light || !scene->ambient_light)
		exit_error("Missing", "light source", cleaner);
	if (!scene->plane && !scene->sphere && !scene->cylinder && !scene->cone)
		exit_error("Missing", "objects", cleaner);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strrchr(const char *s, int c)
{
	char	i;
	char	*end;
	int		len;
	int		flag;

	flag = ft_strlen(s);
	i = (char )c;
	len = ft_strlen(s);
	end = (char *)(s + len);
	while (flag >= 0)
	{
		if (*end == i)
			return (end);
		flag--;
		end--;
	}
	return (NULL);
}
