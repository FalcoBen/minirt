#ifndef MINI_RT_H
#define MINI_RT_H

#include "MLX42/include/MLX42/MLX42.h"
#include "get_next_line/get_next_line.h"
// #include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <libc.h>
#include <stdbool.h>

typedef struct s_container
{
	char *line;
	struct s_container	*next;
}	t_container;
void	ft_lstadd_back(t_container **lst, t_container *new);
void	ft_lstadd_front(t_container **lst, t_container *new);
void	ft_lstclear(t_container **lst, void (*del)(void *));
t_container	*ft_lstnew(void *content);
t_container	*ft_lstlast(t_container *lst);

#endif