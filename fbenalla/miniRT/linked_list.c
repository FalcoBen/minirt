/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:19:20 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 01:20:44 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../../garbage_collector/my_malloc.h"

void	ft_lstadd_back(t_container **lst, t_container *new)
{
	t_container	*end;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	end = ft_lstlast(*lst);
	end->next = new;
}

void	ft_lstadd_front(t_container **lst, t_container *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

void	ft_lstclear(void **lst, void (*del)(void *), char c)
{
	void	*tmp;
	void	*current;

	if (!lst || !*lst || !del)
		return ;
	current = *lst;
	if (c == 'c')
	{
		while (current)
		{
			tmp = ((t_container *)current)->next;
			ft_lstdelone((t_container *)current, del, 'c');
			current = tmp;
		}
	}
	if (c == 'o')
	{
		while (current)
		{
			tmp = ((t_objects_fb *)current)->next;
			ft_lstdelone((t_objects_fb *)current, del, 'o');
			current = tmp;
		}
	}
	*lst = NULL;
}

void	ft_lstdelone(void *lst, void (*del)(void *), char c)
{
	if (!lst || !del)
		return ;
	if (c == 'c')
	{
		del(((t_container *)lst)->line);
	}
	else if (c == 'o')
	{
		del(((t_objects_fb *)lst)->identifier);
	}
	free(lst);
}

t_container	*ft_lstnew(void *content)
{
	t_container	*new;

	new = alloc(sizeof(t_container), false);
	if (!new)
		return (NULL);
	new->line = ft_strdup((char *)content);
	if (!new->line)
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}
//e:               4,555,097 bytes in 259 blocks
//still reachable: 4,555,081 bytes in 257 blocks