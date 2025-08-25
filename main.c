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
// 	// initialize_ambient_light(scene);
// 	// initialize_camera(scene);
// 	// initialize_light(scene);
// 	// initialize_plane(scene);
// 	// initialize_cylinder(scene);
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
	initialize_ambient_light(scene);
	initialize_camera(scene);
	initialize_light(scene);
	initialize_plane(scene);
	initialize_spher(scene);
	initialize_cylinder(scene);
}
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
	char ***tokens = malloc(sizeof(char **) * (counter + 1));
	t_container *curr = head;
	int i = 0;
	while(curr)
	{
		tokens[i] = ft_split_white(curr->line);
		i++;
		curr = curr->next;
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
	t_objects *input_data = malloc(sizeof(t_objects) * (counter + 2));
	t_scene *scene = malloc(sizeof(t_scene));
	init_objects(input_data);
	initialize_scenes(scene);
	int column = 0;
	while(input_data[column].identifier)
	{
		for(int x = 0;x < counter; x++)
		{
			for(int z = 0; tokens[x][z]; z++)
			{
				if(strcmp(tokens[x][0], input_data[column].identifier) == 0)
				{
					input_data[column].assign_object(tokens[x], scene);
				}
			}
		}
		column++;
	}
	for (int i = 0; i < counter; i++)
        ft_free_split(tokens[i]); // Assuming ft_free_split is implemented
    free(tokens);
    ft_lstclear(&head, del); // Free the linked list
    free(input_data);
	// free(scene->ambient_light);
	// free(scene->camera);
	// free(scene->light);
	// free(scene->plane);
	// free(scene->sphere);
	// free(scene->cylinder);
	// free(scene);
	// ft_lstclear(&head, del);

	printf("-----------------------------------------------\n");
	printer(scene);

	return 0;
}
