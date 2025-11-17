#ifndef MY_MALLOC_H
# define MY_MALLOC_H 

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>

typedef struct s_allocater
{
	void				*holder;
	struct s_allocater	*next;
}	t_alloc;

typedef struct s_allocater1
{
	void				*holder;
	struct s_allocater1	*next;
}	t_alloc1;

void	*alloc(size_t size, bool _free);

#endif
