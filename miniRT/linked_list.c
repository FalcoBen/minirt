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
void	ft_lstclear(void **lst, void (*del)(void *), char c)
{
	void	*tmp;
	void	*current;

	if (!lst || !*lst || !del)
		return ;

	current = *lst;
	if(c == 'c')
	{

		while (current)
		{
			tmp = ((t_container *)current)->next;
			ft_lstdelone((t_container *)current, del, 'c');
			current = tmp;
		}
	}
	if(c == 'o')
	{

		while (current)
		{
			tmp = ((t_objects *)current)->next;
			ft_lstdelone((t_objects *)current, del, 'o');
			current = tmp;
		}
	}
	*lst = NULL;
}

void	ft_lstdelone(void *lst, void (*del)(void *), char c)
{
	if (!lst || !del)
		return ;
	if(c == 'c')
	{
		del(((t_container *)lst)->line);
	}
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
