/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 03:56:15 by fbenalla          #+#    #+#             */
/*   Updated: 2024/11/10 04:33:14 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*all;
	t_list	*new;

	if (!lst || !f || !del)
		return (NULL);
	all = NULL;
	while (lst)
	{
		new = ft_lstnew(NULL);
		if (!new)
		{
			ft_lstclear(&all, del);
			return (NULL);
		}
		new -> content = f(lst -> content);
		ft_lstadd_back(&all, new);
		lst = lst->next;
	}
	return (all);
}
