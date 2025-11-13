#include "MiniRt.h"
t_scene *linking_fucntion(t_scene *scene, bool set)
{
    static t_scene *local; 
    if (set == true)
    {
        local = scene;
        return scene;
    }
    return local;
}
void    set_scene(t_scene *scene, bool flag)
{
    static void *holder = NULL; 
    if(!flag)
    {
        puts("--------------------------------------stored ---------------------------------\n");
        holder = scene->cleaner;
    }
    else
    {
        ((t_cleanup *)holder)->flag_input = true;
        exit_error("closing", "window", ((t_cleanup *)holder));
    }
}
void	linked_list_constractor(int fd, t_container **head, int *counter)
{
	char *line = get_next_line(fd);

	t_container *list = NULL;
	while (line != NULL)
    {
		list = ft_lstnew(line);
        ft_lstadd_back(head, list);
        (*counter)++;
        free(line);
        line = get_next_line(fd);
    }
}

void	parsing_scene(int fd, int *counter, t_cleanup *cleaner, t_container *head)
{
	char *line = get_next_line(fd);
	t_container *list = NULL;
	// t_container *head = NULL;
	while (line != NULL)
    {
		list = ft_lstnew(line);
        ft_lstadd_back(&head, list);
        counter++;
        free(line);
        line = get_next_line(fd);
    }       
	close(fd);
}
void 	init_cleaner(t_cleanup *cleaner, t_container *head)
{
	cleaner->container = NULL;
	cleaner->dispatched_table = NULL;
	cleaner->scene = NULL;
	cleaner->token_count = 0;
	cleaner->tokens = NULL;
	cleaner->flag_input = false;
	cleaner->flag_exit = false;
	cleaner->input_data = NULL;
	cleaner->container = head;

}

void 	tokens_constractore(char ***tokens, t_cleanup *cleaner)
{
	t_container *curr = cleaner->container;
	int i = 0;
	while(curr)
	{
		printf("$$$$$%s\n", curr->line);
		tokens[i] = ft_split_white(curr->line);
		i++;
		curr = curr->next;
	}
	cleaner->token_count = i;
	

	int z = 0;
	while(z < i)
	{
			if(!tokens[z][0] || strcmp(tokens[z][0], "A") == 0 || strcmp(tokens[z][0], "C") == 0 || strcmp(tokens[z][0], "L") == 0 \
				|| strcmp(tokens[z][0], "pl") == 0 || strcmp(tokens[z][0], "sp") == 0 ||  strcmp(tokens[z][0], "cy")  == 0 || strcmp(tokens[z][0], "cone")  == 0)
			{
				z++;
				continue;
			}	
			else
				exit_error("object should not render", tokens[z][0], cleaner);
			z++;
	}

}
void	init_input_data(t_objects_fb *input_data)
{
	input_data->assign_object = NULL;
	input_data->identifier = NULL;
	input_data->nb = 0;
	input_data->next = NULL;
}

void	save_objects(char ***tokens, t_objects_fb *dispatch_table, t_scene *scene, int counter)
{
	t_objects_fb *current_obj = dispatch_table;
		for(int x = 0; x < counter; x++)
		{
			if(!tokens[x] || !tokens[x][0]) 
				continue;
			current_obj = dispatch_table;
			while(current_obj)
			{
				if(current_obj->identifier && strcmp(tokens[x][0], current_obj->identifier) == 0)
				{
					current_obj->assign_object(tokens[x], scene);
					break;
				}
				current_obj = current_obj->next;
			}
		}
}

void parsing(int fd)
{
	int counter = 0;
	t_container *head = malloc(sizeof(t_container ));
	head = NULL;
	linked_list_constractor(fd, &head, &counter);
	close(fd);
	t_cleanup *cleaner = malloc(sizeof(t_cleanup));
	init_cleaner(cleaner, head);
	char ***tokens = malloc(sizeof(char **) * (counter + 1));
	cleaner->tokens = tokens;
	tokens_constractore(tokens, cleaner);
		
	t_objects_fb *input_data = malloc(sizeof(t_objects_fb));
	init_input_data(input_data);

	cleaner->input_data = input_data;

	t_objects_fb *dispatch_table = NULL;
	init_object_dispatch_table(&dispatch_table);
	cleaner->dispatched_table = &dispatch_table;


	t_scene *scene = malloc(sizeof(t_scene));
	scene->cleaner = cleaner;
	cleaner->scene = scene;
    initialize_scenes(scene);
	save_objects(tokens, dispatch_table, scene, counter);
	ft_lstclear((void **)&dispatch_table, del, 'o');
	set_scene(scene, false);
	validate_scene(scene);
	t_scene *holder = linking_fucntion(scene, true);
	jassim_mlx(holder);
	// ft_lstclear((void **)&current_obj, del, 'o');
}



