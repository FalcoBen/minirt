#include "minirt.h"

void s()
{
	system("leaks miniRT");
}

bool check_extension(char *str)
{
	char *extension = ".rt";
	char *input_extension = strrchr(str, '.');
	if(strncmp(input_extension, extension, strlen(input_extension)) == 0)
		return true;
	return false;
}

// void	initialize_scenes(t_scene *scene)
// {
	// initialize_ambient_light(scene);
	// initialize_camera(scene);
	// initialize_light(scene);
	// initialize_plane(scene);
	// initialize_cylinder(scene);
// 	scene->ambient_light = malloc(sizeof(scene->ambient_light));
// 	scene->ambient_light->color_ambient_light = malloc(sizeof(scene->ambient_light->color_ambient_light));

// 	scene->camera = malloc(sizeof(scene->camera));
// 	scene->light = malloc(sizeof(scene->light));
// 	scene->plane = malloc(sizeof(scene->plane));
// 	scene->sphere = malloc(sizeof(scene->sphere));
// 	scene->cylinder = malloc(sizeof(scene->cylinder));
// }

void initialize_scenes(t_scene *scene)
{
    scene->ambient_light = NULL;
    scene->camera = NULL;           
    scene->light = NULL;          
    scene->plane = NULL;          
    scene->sphere = NULL;         
    scene->cylinder = NULL;       
}

// void initialize_scenes(t_scene *scene)
// {
	// scene->ambient_light = NULL;
	// scene->camera = NULL;
	// scene->light = NULL;
	// scene->plane = NULL;
	// scene->sphere = NULL;
	// scene->cylinder = NULL;
	// initialize_ambient_light(scene);
	// initialize_camera(scene);
	// initialize_light(scene);
	// initialize_plane(scene);
	// initialize_spher(scene);
	// initialize_cylinder(scene);
	// check_scenes_args(scene);
// }
void del(void *content)
{
    free(content);
}

void	ft_free_split(char **str)
{
	for(int i = 0; str[i]; i++)
		free(str[i]);
	free(str);
}

void	make_sure_of_objects(t_scene *scene)
{
	if (!scene->camera) 
		exit_error("Missing", "camera");
	if (!scene->light || !scene->ambient_light) 
		exit_error("Missing", "light source");
	if (!scene->plane || !scene->sphere || !scene->cylinder) 
		exit_error("Missing", "objects");
	}
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
	t_container *list = NULL;
	t_container *head = NULL;
	char *line = get_next_line(fd);
	int counter = 0;
	while (line != NULL)
    {
        list = ft_lstnew(line);
        ft_lstadd_back(&head, list);
        counter++;
        free(line);
        line = get_next_line(fd);
    }       
	close(fd);
	if(counter == 0)
		exit_error("empty", "file");
	char ***tokens = malloc(sizeof(char **) * (counter + 1));
	t_container *curr = head;
	int i = 0;
	while(curr)
	{
		tokens[i] = ft_split_white(curr->line);
		i++;
		curr = curr->next;
	}
	for(int i = 0; i < counter; i++)
	{
		if(strcmp(tokens[i][0], "A") == 0 || strcmp(tokens[i][0], "C") == 0 || strcmp(tokens[i][0], "L") == 0 || strcmp(tokens[i][0], "pl") == 0 || strcmp(tokens[i][0], "sp") == 0 ||  strcmp(tokens[i][0], "cy") == 0)
			continue;
		else
			exit_error("object should not render", tokens[i][0]);
	}
	t_objects *input_data = malloc(sizeof(t_objects));
	input_data->assign_object = NULL;
	input_data->identifier = NULL;
	input_data->nb = 0;
	input_data->next = NULL;

	// printf("==============counter = %d\n", counter);
	t_objects *dispatch_table = NULL;
    init_object_dispatch_table(&dispatch_table);
    
	t_scene *scene = malloc(sizeof(t_scene));
    initialize_scenes(scene);
    
    for(int x = 0; x < counter; x++)
    {
        if(!tokens[x] || !tokens[x][0]) 
            continue;
            
        t_objects *current_obj = dispatch_table;
        while(current_obj)
        {
            if(current_obj->identifier && strcmp(tokens[x][0], current_obj->identifier) == 0)
            {
                current_obj->assign_object(tokens[x], scene);
                break;
            }
			// else
			// {
			// 	for(int i = 0; i < counter; i++)
			// 	{
			// 		if(strcmp(tokens[i][0], current_obj->identifier) != 0)
			// 			exit_error("object should not render", tokens[x][0]);
			// 	}
			// }
            current_obj = current_obj->next;
        }
    }

	make_sure_of_objects(scene);




	for (int i = 0; i < counter; i++)
        ft_free_split(tokens[i]);
    free(tokens);
    ft_lstclear(&head, del);
    free(input_data);

	printf("-----------------------------------------------\n");
	printer(scene);

	return 0;
}
	// int x = 0;
	// int z = 0;
	// while(x < counter)
	// {
	// 	z = 0;
	// 	while(tokens[x][z])
	// 	{
	// 		printf("token[%i][%i] = %s\n", x, z, tokens[x][z]);
	// 		z++;
	// 	}
	// 	x++;
	// }
	// tokens[x] = NULL;



	// while(a < counter)
	// {
	// 	printf("[[[%s]]], [[%d]]\n", input_data[a].identifier, input_data[a].nb);
	// 	// input_data = input_data->next;
	// 	a++;
	// }