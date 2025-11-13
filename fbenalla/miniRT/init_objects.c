#include "parsing.h"

void    objects_linking_constractor(char *identifier, void (*assign_object)(char **, t_scene *), t_objects_fb **head, t_objects_fb *current)
{
	t_objects_fb *new_node = malloc(sizeof(t_objects_fb));
	if (!new_node)
		return;

	new_node->identifier = ft_strdup(identifier);
	new_node->assign_object = assign_object;
	new_node->next = NULL;
	if (*head == NULL)
	{
		*head = new_node;
	}
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}

}
void add_object(t_objects_fb **head, char *identifier, void (*assign_object)(char **, t_scene *))
{
	t_objects_fb *current = *head;
	while (current)
	{
		if (current->identifier && strcmp(current->identifier, identifier) == 0)
		{
			return;
		}
		current = current->next;
	}
	objects_linking_constractor(identifier, assign_object, head, current);

}

void init_object_dispatch_table(t_objects_fb **dispatch_table)
{
	add_object(dispatch_table, "A", ft_ambient_light_fb);
	add_object(dispatch_table, "C", ft_camera_fb);
	add_object(dispatch_table, "L", ft_light);
	add_object(dispatch_table, "pl", ft_plane_fb);
	add_object(dispatch_table, "sp", ft_sphere_fb);
	add_object(dispatch_table, "cy", ft_cylinder_fb);
	add_object(dispatch_table, "cone", ft_cone_fb);
}

bool	check_colors_args(char **colors)
{
	int i = 0;
	while(colors[i])
		i++;
	if (i != 3)
		return false;
	return true;
}

void	check_valid_args(char **data, t_scene *scene, char object)
{
	int i = 0;
	i = 0;
	while (data[i])
		i++;
	if (object == 'A')
	{
		if (i != 3)
			exit_error("invalid arguments", "in A", scene->cleaner);
	}
	if (object == 'C')
	{
		if (i != 4)
			exit_error("invalid arguments", "in C", scene->cleaner);
	}
	if (object == 'L')
	{
		if (i != 4)
			exit_error("invalid arguments", "in L", scene->cleaner);
	}
}
void	ambient_light_linked_list(t_ambient_light_fb *new_ambient, t_scene *scene)
{
	t_ambient_light_fb	*current;

	if (scene->ambient_light == NULL)
		scene->ambient_light = new_ambient;
	else
	{
		current = scene->ambient_light;
		while (current->next)
			current = current->next;
		current->next = new_ambient;
	}
	t_ambient_light_fb *tmp = scene->ambient_light;
	int counter = 0;
	while (tmp)
	{
		counter++;
		tmp = tmp->next;
	}
	if (counter > 1)
		exit_error("duplicate object", "ambient_light", scene->cleaner);
}
void	camera_linked_list(t_camera_fb *new_camera, t_scene *scene)
{
	t_camera_fb	*current;

	if (scene->camera == NULL)
		scene->camera = new_camera;
	else
	{
		current = scene->camera;
		while(current->next)
			current = current->next;
		current->next = new_camera;
	}

	t_camera_fb *tmp = scene->camera;
	int counter = 0;
	while (tmp)
	{
		counter++;
		tmp = tmp->next;
	}
	if (counter > 1)
		exit_error("duplicate object", "camera", scene->cleaner);
}
void	light_linked_list(t_light_fb *new_light, t_scene *scene)
{

	t_light_fb *current;
	if (scene->light == NULL)
		scene->light = new_light;
	else
	{
		current = scene->light;
		while(current->next)
			current = current->next;
		current->next = new_light;
	}
}


bool	verify_data_ambient_light(char **data, t_cleanup *clean)
{
	int i = 0;
	if (!data[1] || !data[2])
	{
		return (false);
	}
	double amient = ft_atoi_double(data[1], clean);
	if (clean->flag_exit)
	{
		printf("333333333333333344444444444444444444\n");
		return (false);
	}
	if (!verify_color(data[2]))
	{
		printf("22222222222222222222222222222222222222\n");
		return (false);
	}
	
	while(data[i])
		i++;
	if (i > 3)
	{
		printf("alot of arguments in ambient_light\n");
		return (false);
	}
	return (true);
}

void	ft_ambient_light_fb(char **data, t_scene *scene)
{	
	t_ambient_light_fb	*new_ambient;
	char			**colors;
	
	check_valid_args(data, scene, 'A');
	if(!verify_data_ambient_light(data, scene->cleaner))
		exit_error("data not in the correct format", "in ambient_light", scene->cleaner);
	new_ambient = malloc(sizeof(t_ambient_light_fb));
	new_ambient->color_ambient_light = malloc(sizeof(t_color_fb));
	new_ambient->next = NULL;

	new_ambient->bright_ambient_light_fb = ft_atoi_double(data[1], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in", "A", scene->cleaner);
	if (new_ambient->bright_ambient_light_fb < 0 || new_ambient->bright_ambient_light_fb > 1)
		exit_error("invalid range ambient light", "A", scene->cleaner);
	if (count_comma(data[2]) != 2)
		exit_error("more or less in color", "in ambient_light", scene->cleaner);
	colors = ft_split(data[2], ',');
	new_ambient->color_ambient_light->r = ft_atoi_color(colors[0], "ambient light r");
	new_ambient->color_ambient_light->g = ft_atoi_color(colors[1], "ambient light g");
	new_ambient->color_ambient_light->b = ft_atoi_color(colors[2], "ambient light b");
	ambient_light_linked_list(new_ambient, scene);
	ft_free_split(colors);
	
}


void	coor_camera(t_camera_fb *new_camera, char **data, t_scene *scene)
{
	new_camera->coor_camera = malloc(sizeof(t_vec3));
	char **coors = ft_split(data[1], ',');
	new_camera->coor_camera->x = ft_atoi_double(coors[0], scene->cleaner);
	new_camera->coor_camera->y = ft_atoi_double(coors[1], scene->cleaner);
	new_camera->coor_camera->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coors", "C", scene->cleaner);
	ft_free_split(coors);
	
}
void	vector_camera(t_camera_fb *new_camera, char **data, t_scene *scene)
{
	new_camera->vector_camera = malloc(sizeof(t_vec3));
	char **vects = ft_split(data[2], ',');
	new_camera->vector_camera->x = ft_atoi_double(vects[0], scene->cleaner);
	new_camera->vector_camera->y = ft_atoi_double(vects[1], scene->cleaner);
	new_camera->vector_camera->z = ft_atoi_double(vects[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in vectors", "C", scene->cleaner);
	double range_x = new_camera->vector_camera->x;
	double range_y = new_camera->vector_camera->y;
	double range_z = new_camera->vector_camera->z;
	if (range_x < -1 || range_x > 1)
		exit_error("invalid range vectors", "C in x", scene->cleaner);

	if (range_y < -1 || range_y > 1)
		exit_error("invalid range vectors", "C in y", scene->cleaner);
		
	if (range_z < -1 || range_z > 1)
		exit_error("invalid range vectors", "C in z", scene->cleaner);
	ft_free_split(vects);
	
}
bool	verify_data_camera(char **data, t_cleanup *clean)
{
	int i = 0;
	if (!data[1] || !data[2] || !data[3])
	{
		return (false);
	}
	if (!is_coordinate(data[1]))
	{
		printf("111111111111111111111111111111111111\n");
		return (false);
	}
	if (!is_coordinate(data[2]))
	{
		printf("222222222222222222222222222222222222222222222\n");
		return (false);
	}
	double angle = ft_atoi_double(data[3], clean);
	if (clean->flag_exit)
	{
		printf("333333333333333344444444444444444444\n");
		return (false);
	}
	while(data[i])
		i++;
	if (i > 4)
	{
		printf("alot of arguments in light\n");
		return (false);
	}
	return (true);
}
void	ft_camera_fb(char **data, t_scene *scene)
{
	check_valid_args(data, scene, 'C');
	if(!verify_data_camera(data, scene->cleaner))
		exit_error("data not in the correct format", "in camera", scene->cleaner);
	t_camera_fb *new_camera = malloc(sizeof(t_camera_fb));
	t_camera_fb *current;
	new_camera->next = NULL;
	coor_camera(new_camera, data, scene);
	vector_camera(new_camera, data, scene);
	new_camera->angle_view = ft_atoi_double(data[3], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in angle", "C", scene->cleaner);
	if (new_camera->angle_view < 0 || new_camera->angle_view > 180)
		exit_error("invalid range angle view", "C", scene->cleaner);
	camera_linked_list(new_camera, scene);
}

void	coor_light(t_light_fb *new_light, char **data, t_scene *scene)
{
	new_light->coor_light = malloc(sizeof(t_vec3));

	char **coors = ft_split(data[1], ',');
	new_light->coor_light->x = ft_atoi_double(coors[0], scene->cleaner);
	new_light->coor_light->y = ft_atoi_double(coors[1], scene->cleaner);
	new_light->coor_light->z = ft_atoi_double(coors[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "L", scene->cleaner);
	new_light->bright_light = ft_atoi_double(data[2], scene->cleaner);
	if (scene->cleaner->flag_exit)
		exit_error("invalid thing in bright_light", "L", scene->cleaner);
	if (new_light->bright_light < 0 || new_light->bright_light > 1)
		exit_error("invalid range bright light", "L", scene->cleaner);
	if (count_comma(data[3]) != 2)
		exit_error("more or less in color", "in light", scene->cleaner);
	ft_free_split(coors);
	
}
bool	verify_data_light(char **data, t_cleanup *clean)
{
	int i = 0;
	if (!data[1] || !data[2] || !data[3])
	{
		return (false);
	}
	if (!is_coordinate(data[1]))
	{
		printf("111111111111111111111111111111111111\n");
		return (false);
	}
	double bright = ft_atoi_double(data[2], clean);
	if (clean->flag_exit)
	{
		printf("333333333333333344444444444444444444\n");
		return (false);
	}
	if (!verify_color(data[3]))
	{
		printf("333333333333333333333333333333333333333333333333333\n");
		return (false);
	}
	while(data[i])
		i++;
	if (i > 4)
	{
		printf("alot of arguments in light\n");
		return (false);
	}
	return (true);
}
 
void	ft_light(char **data, t_scene *scene)
{
	t_light_fb *new_light;

	check_valid_args(data, scene, 'L');
	if(!verify_data_light(data, scene->cleaner))
		exit_error("data not in the correct format", "in light", scene->cleaner);
	new_light = malloc(sizeof(t_light_fb));
	new_light->next = NULL;
	coor_light(new_light, data, scene);
	new_light->color_light = malloc(sizeof(t_color_fb));
	char **colors = ft_split(data[3], ',');
	new_light->color_light->r = ft_atoi_color(colors[0], "light");
	new_light->color_light->g = ft_atoi_color(colors[1], "light");
	new_light->color_light->b = ft_atoi_color(colors[2], "light");
	if (new_light->color_light->r == -1 || new_light->color_light->g == -1 ||new_light->color_light->b == -1)
		exit_error("somthing wrong with colors", "in light", scene->cleaner);
	light_linked_list(new_light, scene);
	ft_free_split(colors);
}
