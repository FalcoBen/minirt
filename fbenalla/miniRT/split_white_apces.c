/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_white_apces.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:28:56 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:22:38 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

bool	ispace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (true);
	return (false);
}

int	count_word(char *str)
{
	int	i;
	int	flag;
	int	counter;

	i = 0;
	flag = 0;
	counter = 0;
	while (str[i])
	{
		if (ispace(str[i]))
			flag = 0;
		else if (flag == 0)
		{
			flag = 1;
			counter++;
		}
		i++;
	}
	return (counter);
}

int	lenght_word(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '\t')
		i++;
	return (i);
}

char	**ft_split_white(char *str)
{
	int		counts;
	char	**arr;
	int		i;

	counts = count_word(str);
	arr = alloc(sizeof(char *) * (counts + 1), false);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < counts)
	{
		while (*str == ' ' || (*str >= 9 && *str <= 13))
			str++;
		arr[i] = alloc((sizeof(char ) * lenght_word(str) + 1), false);
		if (!arr[i])
			return (NULL);
		ft_strlcpy(arr[i], str, lenght_word(str) + 1);
		str = str + lenght_word(str);
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
