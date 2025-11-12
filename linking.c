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

void parsing(int fd)
{
	int counter = 0;
	// char *line = get_next_line(fd);
	// t_container *list = NULL;
	t_container *head = NULL;
	// while (line != NULL)
    // {
	// 	list = ft_lstnew(line);
    //     ft_lstadd_back(&head, list);
    //     counter++;
    //     free(line);
    //     line = get_next_line(fd);
    // }       
	// close(fd);
	// cleaner->container = NULL;
	// cleaner->dispatched_table = NULL;
	// cleaner->scene = NULL;
	// cleaner->token_count = 0;
	// cleaner->tokens = NULL;
	// cleaner->flag_input = false;
	// cleaner->flag_exit = false;
	// cleaner->input_data = NULL;
	// cleaner->container = head;
	t_cleanup *cleaner = malloc(sizeof(t_cleanup));
	parsing_scene(fd, &counter, cleaner, head);

	char ***tokens = malloc(sizeof(char **) * (counter + 1));
	cleaner->tokens = tokens;
	t_container *curr = head;
	int i = 0;
	while(curr)
	{
		tokens[i] = ft_split_white(curr->line);
		i++;
		curr = curr->next;
	}
	cleaner->token_count = i;
	

	int z = 0;
	while(z < counter)
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
		
	t_objects_fb *input_data = malloc(sizeof(t_objects_fb));
	input_data->assign_object = NULL;
	input_data->identifier = NULL;
	input_data->nb = 0;
	input_data->next = NULL;
	cleaner->input_data = input_data;
	t_objects_fb *dispatch_table = NULL;
	init_object_dispatch_table(&dispatch_table);
	cleaner->dispatched_table = &dispatch_table;
	t_scene *scene = malloc(sizeof(t_scene));
	scene->cleaner = cleaner;
	cleaner->scene = scene;
    initialize_scenes(scene);
    
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
	ft_lstclear((void **)&dispatch_table, del, 'o');
	set_scene(scene, false);
	validate_scene(scene);
	// printer(scene);
	t_scene *holder = linking_fucntion(scene, true);
	jassim_mlx(holder);
}



