#include "../../MiniRt.h"
#include "../../garbage_collector/my_malloc.h"

void	reading_from_scene(int fd, t_container **head, int *counter)
{
	char		*line;
	t_container	*list;

	line = get_next_line(fd);
	list = NULL;
	while (line != NULL)
	{
		list = ft_lstnew(line);
		ft_lstadd_back(head, list);
		(*counter)++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

void	stor_tokens(char ***tokens, t_container *curr, t_cleanup *cleaner)
{
	int	i;

	i = 0;
	while (curr)
	{
		tokens[i] = ft_split_white(curr->line);
		i++;
		curr = curr->next;
	}
}

void	tokens_constractor(char ***tokens, t_container *head, \
		int counter, t_cleanup *cleaner)
{
	t_container	*curr;
	int			i;
	int			z;

	curr = head;
	i = 0;
	stor_tokens(tokens, curr, cleaner);
	z = 0;
	while (z < counter)
	{
		if (!tokens[z][0] || ft_strcmp(tokens[z][0], "A") == 0 || \
		ft_strcmp(tokens[z][0], "C") == 0 || ft_strcmp(tokens[z][0], "L") == 0 \
			|| ft_strcmp(tokens[z][0], "pl") == 0 || \
			ft_strcmp(tokens[z][0], "sp") == 0 || ft_strcmp(tokens[z][0], "cy") \
					== 0 || ft_strcmp(tokens[z][0], "cone") == 0)
		{
			z++;
			continue ;
		}
		else
			exit_error("object should not render", tokens[z][0], cleaner);
		z++;
	}
}

void	start_asigning_objects(t_scene *scene, \
		t_objects_fb *dispatch_table, char ***tokens, int counter)
{
	t_objects_fb	*current_obj;
	int				x;

	current_obj = dispatch_table;
	x = 0;
	while (x < counter)
	{
		if (!tokens[x] || !tokens[x][0])
		{
			x++;
			continue ;
		}
		current_obj = dispatch_table;
		while (current_obj)
		{
			if (current_obj->identifier && \
				ft_strcmp(tokens[x][0], current_obj->identifier) == 0)
			{
				current_obj->assign_object(tokens[x], scene);
				break ;
			}
			current_obj = current_obj->next;
		}
		x++;
	}
}

void	parsing(int fd)
{
	int			counter;
	t_container	*head;
	t_cleanup	*cleaner;
	char		***tokens;

	head = NULL;
	counter = 0;
	reading_from_scene(fd, &head, &counter);
	cleaner = alloc(sizeof (t_cleanup), false);
	init_cleaner(cleaner);
	tokens = alloc(sizeof (char **) * ((counter) + 1), false);
	tokens_constractor(tokens, head, counter, cleaner);
	parsing_second_part(tokens, &counter, cleaner);
}
