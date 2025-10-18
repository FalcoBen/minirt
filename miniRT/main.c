#include "minirt.h"

void s()
{
	system("leaks miniRT");
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

// void	free_color(t_color *color)
// {
// 	free(color->r);
// 	free(color->g);
// 	free(color->b);
// }

// void	free_coor(t_vec3 *coor)
// {
// 	free(coor);
// 	free(coor->x);
// 	free(coor->y);
// 	free(coor->z);
// }

void	free_scene(t_scene *scene)
{
	if(scene->ambient_light)
	{
		t_ambient_light *amb = scene->ambient_light;
		free(amb->color_ambient_light);
		free(amb);
	}
	if(scene->sphere)
	{
		t_sphere *sp = scene->sphere;
		while(sp)
		{
			free(sp->color_sphere);
			free(sp->coor_sphere);
			free(sp);
			sp = sp->next;
		}
	}
	if(scene->camera)
	{
		t_camera *cm = scene->camera;
		free(cm->coor_camera);
		free(cm->vector_camera);
		free(cm);
	}
	if(scene->plane)
	{
		t_plane *pl = scene->plane;
		while(pl)
		{
			free(pl->color_plane);
			free(pl->coor_plane);
			free(pl->vector_plane);
			free(pl);
			pl = pl->next;
		}
	}
	if(scene->cylinder)
	{
		t_cylinder *cy = scene->cylinder;
		while(cy)
		{
			free(cy->color_cylinder);
			free(cy->coor_cylinder);
			free(cy->vector_cylinder);
			free(cy);
			cy = cy->next;
		}
	}
	free(scene);
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
		exit_error("empty", "file", &head, false);
	char ***tokens = malloc(sizeof(char **) * (counter + 1));
	t_container *curr = head;
	int i = 0;
	while(curr)
	{
		tokens[i] = ft_split_white(curr->line);
		i++;
		curr = curr->next;
	}
	exit_error("cleanup", "container", &head, true);
	
	// for(int i = 0; i < counter; i++)
	// {
	// 	if(strcmp(tokens[i][0], "A") == 0 || strcmp(tokens[i][0], "C") == 0 || strcmp(tokens[i][0], "L") == 0 || strcmp(tokens[i][0], "pl") == 0 || strcmp(tokens[i][0], "sp") == 0 ||  strcmp(tokens[i][0], "cy") == 0)
	// 		continue;
	// 	else
	// 		//exit_error("object should not render", tokens[i][0]);
	// }
	t_objects *input_data = malloc(sizeof(t_objects));
	input_data->assign_object = NULL;
	input_data->identifier = NULL;
	input_data->nb = 0;
	input_data->next = NULL;
	
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
            current_obj = current_obj->next;
        }
    }
	free(input_data);
	// make_sure_of_objects(scene);
	// // printer(scene);

	// // printf("[[[[[[[[[[[[[[[[[[[[[%f]]]]]]]]]]]]]]]]]]]]]]]]\n", scene->sphere->coor_sphere->x);
	// // // test_vector_math();
	t_world z =	s_world(scene);
	// for(int i = 0; z.objects[i] != NULL; i++)
	// {
	// 	printf("______________________________%d_____________________\n", z.objects[i]->obj->sphere_ja->id);

	// }
	print_world(&z);
	// start_using_mlx(scene);
	
	
	for (int i = 0; i < counter; i++)
        ft_free_split(tokens[i]);
    free(tokens);

	free_scene(scene);
	// // printf("-----------------------------------------------\n");
	// // printer(scene);

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


	