#include "parsing.h"
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
    scene->cone = NULL;       
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
	parsing(fd);

	
	// ft_lstclear((void **)&current_obj, del, 'o');


	/*----------------------------------------------------------------*/
	// if(cleaner->flag_input)
	// {
		
	// 	for (int i = 0; i < counter; i++)
	// 		ft_free_split(tokens[i]);
	// 	free(tokens);
	// 	// free(cleaner);
	// 	free_scene(scene);
	// 	free(cleaner);
	// 	ft_lstclear((void **)&head, del, 'c');
	// 	free(input_data);
	// }
	/*----------------------------------------------------------------*/

	// // printf("-----------------------------------------------\n");
	// // printer(scene);

	return 0;
}
