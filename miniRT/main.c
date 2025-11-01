#include "minirt.h"

bool check_extension(char *str)
{
	char *extension = ".rt";
	char *input_extension = strrchr(str, '.');
	if(strncmp(input_extension, extension, strlen(input_extension)) == 0)
		return true;
	return false;
}

void initialize_scenes(t_scene *scene)
{
    scene->ambient_light = NULL;
    scene->camera = NULL;           
    scene->light = NULL;          
    scene->plane = NULL;          
    scene->sphere = NULL;         
    scene->cylinder = NULL;       
}



void	make_sure_of_objects(t_scene *scene)
{
	// if (!scene->camera) 
	// 	exit_error("Missing", "camera");
	// if (!scene->light || !scene->ambient_light) 
	// 	exit_error("Missing", "light source");
	// if (!scene->plane && !scene->sphere && !scene->cylinder) 
	// 	exit_error("Missing", "objects");
}
// void	free_identifier(t_objects *dispatch_table)
// {
// 	while (dispatch_table)
// 	{
// 		free(dispatch_table->identifier);
// 		dispatch_table = dispatch_table->next;
// 	}
	
// }

int main(int ac, char **av)
{
	// atexit(s);
	if(ac != 2)
		return 1;
	char *file_name = av[1];
	if(!check_extension(file_name))
	{
		printf("Error\n, bad extension\n");
		return 1;
	}
	int fd = open(file_name, O_RDONLY, 0644);
	if(fd < 0)
	{
		printf("failed to open file\n");
		return 2;
	}
	char *line = get_next_line(fd);
	int counter = 0;
	t_container *list = NULL;
	t_container *head = NULL;
	while (line != NULL)
    {
		list = ft_lstnew(line);
        ft_lstadd_back(&head, list);
        counter++;
        free(line);
        line = get_next_line(fd);
    }       
	close(fd);
	t_cleanup *cleaner = malloc(sizeof(t_cleanup));
	cleaner->container = NULL;
	cleaner->dispatched_table = NULL;
	cleaner->scene = NULL;
	cleaner->token_count = 0;
	cleaner->tokens = NULL;
	cleaner->flag_input = false;
	cleaner->flag_exit = false;
	cleaner->input_data = NULL;
	
	cleaner->container = head; //////////////////////////
	if(counter == 0)
	{
		// exit_error("empty", "file", &head, 'e');
		exit_error("empty", "file", cleaner);
		return 1;
	}
	char ***tokens = malloc(sizeof(char **) * (counter + 1));
	cleaner->tokens = tokens; ////////////////
	t_container *curr = head;
	int i = 0;
	while(curr)
	{
		tokens[i] = ft_split_white(curr->line);
		i++;
		curr = curr->next;
	}
	cleaner->token_count = i; /////////////////
	
	t_scene *scene = malloc(sizeof(t_scene));

	int z = 0;
	while(z < counter)
	{
			if(!tokens[z][0] || strcmp(tokens[z][0], "A") == 0 || strcmp(tokens[z][0], "C") == 0 || strcmp(tokens[z][0], "L") == 0 || strcmp(tokens[z][0], "pl") == 0 || strcmp(tokens[z][0], "sp") == 0 ||  strcmp(tokens[z][0], "cy")  == 0)
			{
				z++;
				continue;
			}	
			else
				exit_error("object should not render", tokens[z][0], cleaner);
			z++;
	}
		
	t_objects *input_data = malloc(sizeof(t_objects));
	input_data->assign_object = NULL;
	input_data->identifier = NULL;
	input_data->nb = 0;
	input_data->next = NULL;
	cleaner->input_data = input_data;
	t_objects *dispatch_table = NULL;
	init_object_dispatch_table(&dispatch_table);
	// free_identifier(dispatch_table);

	// t_scene *scene = malloc(sizeof(t_scene));
	cleaner->dispatched_table = &dispatch_table;
	scene->cleaner = cleaner;
	cleaner->scene = scene;
    initialize_scenes(scene);
    
	t_objects *current_obj = dispatch_table;
    for(int x = 0; x < counter; x++)
    {
		if(!tokens[x] || !tokens[x][0]) 
			continue;
		
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
	// make_sure_of_objects(scene);
	
	// printer(scene);
	/*---------------------------------------------jassim-------------------------------------------------*/
	// t_world z =	s_world(scene);
	// print_world(&z);
	printf("******************************\n");
	ft_lstclear((void **)&dispatch_table, del, 'o');
	// ft_lstclear((void **)&head, del, 'c');
	set_scene(scene, false);
	start_using_mlx(scene);
	
	// printf("caller: dispatch_table = %p, &dispatch_table = %p\n", (void *)dispatch_table, (void *)&dispatch_table);
	// if (dispatch_table)
	// 	printf("caller: first->next = %p, first->identifier = %p\n", (void *)dispatch_table->next, (void *)dispatch_table->identifier);
	
	// ft_lstclear((void **)&current_obj, del, 'o');
	if(cleaner->flag_input)
	{
		
		for (int i = 0; i < counter; i++)
		ft_free_split(tokens[i]);
		free(tokens);
		// free(cleaner);
		free_scene(scene);
		free(cleaner);
		ft_lstclear((void **)&head, del, 'c');
		free(input_data);
	}

	// // printf("-----------------------------------------------\n");
	// // printer(scene);

	return 0;
}
