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
	while(line != NULL)
	{
		list = ft_lstnew(line);
		// free(line);
		ft_lstadd_back(&head, list);
		counter++;
		line = get_next_line(fd);
	}         
	char ***tokens = malloc(sizeof(char **) * (counter + 1));
	t_container *curr = head;
	int i = 0;
	while(curr)
	{
		tokens[i] = ft_split_white(curr->line);
		i++;
		curr = curr->next;
	}
	int x = 0;
	int z = 0;
	while(x < counter)
	{
		z = 0;
		while(tokens[x][z])
		{
			printf("token[%i][%i] = %s\n", x, z, tokens[x][z]);
			z++;
		}
		x++;
	}
	tokens[x] = NULL;
	t_objects *input_data = malloc(sizeof(t_objects) * (counter + 2));
	t_scene *scene = malloc(sizeof(t_scene));
	init_objects(input_data);
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
	}
	return 0;
}
