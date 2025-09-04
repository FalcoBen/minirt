#include "minirt.h"

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
void	ft_lstclear(t_container **lst, void (*del)(void *))
{
	t_container	*tmp;

	if (!lst || !del)
		return ;
	while ((*lst))
	{
		tmp = (*lst)->next;
		ft_lstdelone((*lst), del);
		*lst = tmp;
	}
	*lst = NULL;
}
void	ft_lstdelone(t_container *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->line);
	free(lst);
}

t_container *ft_lstnew(void *content)
{
    t_container *new;

    new = malloc(sizeof(t_container));
    if (!new)
        return (NULL);
    new->line = strdup((char *)content);
    if (!new->line)
    {
        free(new);
        return (NULL);
    }
    new->next = NULL;
    return (new);
}
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
