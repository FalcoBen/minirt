#include "minirt.h"

void add_object(t_objects **head, char *identifier, void (*assign_object)(char **, t_scene *))
{
    t_objects *new_node = malloc(sizeof(t_objects));
    if (!new_node)
        return;

    new_node->identifier = strdup(identifier);
    new_node->assign_object = assign_object;
    new_node->next = NULL;
    new_node->nb = 1;

    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        t_objects *current = *head;
        while (current->next)
        {
            current = current->next;
        }
        current->next = new_node;
        (*head)->nb += 1;
    }
}
void init_objects(t_objects **input_data, char ***tokens, int counter)
{
    int i = 0;
    
    while(tokens[i] && i < counter)
    {
        if(strcmp(tokens[i][0], "A") == 0)
        {
            add_object(input_data, "A", ft_ambient_light);
        }
        else if(strcmp(tokens[i][0], "C") == 0)
        {
            add_object(input_data, "C", ft_camera);
        }
        else if(strcmp(tokens[i][0], "L") == 0)
        {
            add_object(input_data, "L", ft_light);
        }
        else if(strcmp(tokens[i][0], "pl") == 0)
        {
            add_object(input_data, "pl", ft_plane);
        }
        else if(strcmp(tokens[i][0], "sp") == 0)
        {
            add_object(input_data, "sp", ft_sphere);
        }
        else if(strcmp(tokens[i][0], "cy") == 0)
        {
            add_object(input_data, "cy", ft_cylinder);
        }
        i++;
    }
}
bool	check_colors_args(char **colors)
{
	int i = 0;
	while(colors[i])
		i++;
	if(i != 3)
		return false;
	return true;
}

void ft_ambient_light(char **data, t_scene *scene)
{
    int i = 0;
    while(data[i])
        i++;
    if(i != 3)
        exit_error("invalid arguments", "in A");

    // Create new ambient light node
    t_ambient_light *new_ambient = malloc(sizeof(t_ambient_light));
    new_ambient->color_ambient_light = malloc(sizeof(t_color));
    new_ambient->next = NULL;
    
    new_ambient->bright_ambient_light = ft_atoi_double(data[1]);
    if(new_ambient->bright_ambient_light < 0 || new_ambient->bright_ambient_light > 1)
        exit_error("invalid range ambient light", "A");

    char **colors = ft_split(data[2], ',');
    if(!check_colors_args(colors))
        exit_error("some color is missing", "in A");
    
    new_ambient->color_ambient_light->r = ft_atoi_color(colors[0], "ambient light r");
    new_ambient->color_ambient_light->g = ft_atoi_color(colors[1], "ambient light g");
    new_ambient->color_ambient_light->b = ft_atoi_color(colors[2], "ambient light b");

    if(scene->ambient_light == NULL)
    {
        scene->ambient_light = new_ambient;
    }
    else
    {
        t_ambient_light *current = scene->ambient_light;
        // FIX: Check if current is not NULL before accessing next
        while(current && current->next)
            current = current->next;
        current->next = new_ambient;
    }
}

// void	ft_ambient_light(char **data, t_scene *scene)
// {
// 	int i = 0;
// 	if(!scene->ambient_light)
// 	{	
// 		puts("-----------------------filled-----------------------\n");
// 		// ft_ambient_light(data, scene);
// 		// scene = scene->next;
// 	}
// 	while(data[i])
// 		i++;
// 	if(i != 3)
// 		exit_error("invalid arguments", "in L");
// 	scene->ambient_light->bright_ambient_light = ft_atoi_double(data[1]);
// 	if(scene->ambient_light->bright_ambient_light < 0 || scene->ambient_light->bright_ambient_light > 1)
// 		exit_error("invalid range ambient light", "A");

// 	char **colors = ft_split(data[2], ',');
// 	if(!check_colors_args(colors))
// 		exit_error("some color is missing", "in A");
// 	scene->ambient_light->color_ambient_light->r = ft_atoi_color(colors[0], "ambient light r");
// 	scene->ambient_light->color_ambient_light->g = ft_atoi_color(colors[1], "ambient light g");
// 	scene->ambient_light->color_ambient_light->b = ft_atoi_color(colors[2], "ambient light b");
	
// }
void	ft_camera(char **data, t_scene *scene)
{
	int i;
	for(i = 0; data[i]; i++)
		i++;
	if(i != 4)
		exit_error("invalid arguments", "in C");
	char **coors = ft_split(data[1], ',');
	scene->camera->coor_camera->x = ft_atoi_double(coors[0]);
	scene->camera->coor_camera->y = ft_atoi_double(coors[1]);
	scene->camera->coor_camera->z = ft_atoi_double(coors[2]);
	char **vects = ft_split(data[2], ',');
	scene->camera->vector_camera->x = ft_atoi_double(vects[0]);
	scene->camera->vector_camera->y = ft_atoi_double(vects[1]);
	scene->camera->vector_camera->z = ft_atoi_double(vects[2]);
	double range_x = scene->camera->vector_camera->x;
	double range_y = scene->camera->vector_camera->y;
	double range_z = scene->camera->vector_camera->z;
	if(range_x < -1 || range_x > 1)
		exit_error("invalid range vectors", "C in x");

	if(range_y < -1 || range_y > 1)
		exit_error("invalid range vectors", "C in y");
		
	if(range_z < -1 || range_z > 1)
		exit_error("invalid range vectors", "C in z");
	scene->camera->angle_view = ft_atoi_double(data[3]);
	if(scene->camera->angle_view < 0 || scene->camera->angle_view > 180)
		exit_error("invalid range angle view", "C");

}
void	ft_light(char **data, t_scene *scene)
{
	int i;
	for(i = 0; data[i]; i++)
		i++;
	if(i != 4)
		exit_error("invalid arguments", "in L");
	char **coors = ft_split(data[1], ',');
	scene->light->coor_light->x = ft_atoi_double(coors[0]);
	scene->light->coor_light->y = ft_atoi_double(coors[1]);
	scene->light->coor_light->z = ft_atoi_double(coors[2]);

	scene->light->bright_light = ft_atoi_double(data[2]);
	if(scene->light->bright_light < 0 || scene->light->bright_light > 1)
		exit_error("invalid range bright light", "L");
	char **colors = ft_split(data[3], ',');
	if(!check_colors_args(colors))
		exit_error("some color is missing", "in LIGHT");
	scene->light->color_light->r = ft_atoi_color(colors[0], "light");
	scene->light->color_light->g = ft_atoi_color(colors[1], "light");
	scene->light->color_light->b = ft_atoi_color(colors[2], "light");
}
void	ft_plane(char **data, t_scene *scene)
{
	int i;
	for(i = 0; data[i]; i++)
		i++;
	if(i != 4)
		exit_error("invalid arguments", "in pl");
	char **coors = ft_split(data[1], ',');
	scene->plane->coor_plane->x = ft_atoi_double(coors[0]);
	scene->plane->coor_plane->y = ft_atoi_double(coors[1]);
	scene->plane->coor_plane->z = ft_atoi_double(coors[2]);

	char **vects = ft_split(data[2], ',');
	scene->plane->vector_plane->x = ft_atoi_double(vects[0]);
	scene->plane->vector_plane->y = ft_atoi_double(vects[1]);
	scene->plane->vector_plane->z = ft_atoi_double(vects[2]);

	double range_x = scene->plane->vector_plane->x;
	double range_y = scene->plane->vector_plane->y;
	double range_z = scene->plane->vector_plane->z;
	if(range_x < -1 || range_x > 1 || range_y < -1 || range_y > 1 || range_z < -1 || range_z > 1)
			exit_error("invalid range vectors", "PL");

	char **colors = ft_split(data[3], ',');
	if(!check_colors_args(colors))
		exit_error("some color is missing", "in PL");
	scene->plane->color_plane->r = ft_atoi_color(colors[0], "plane");
	scene->plane->color_plane->g = ft_atoi_color(colors[1], "plane");
	scene->plane->color_plane->b = ft_atoi_color(colors[2], "plane");

}
void	ft_sphere(char **data, t_scene *scene)
{
	int i;
	for(i = 0; data[i]; i++)
		i++;
	if(i != 4)
		exit_error("invalid arguments", "in SP");
	char **coors = ft_split(data[1], ',');
	scene->sphere->coor_sphere->x = ft_atoi_double(coors[0]);
	scene->sphere->coor_sphere->y = ft_atoi_double(coors[1]);
	scene->sphere->coor_sphere->z = ft_atoi_double(coors[2]);

	scene->sphere->diameter_sphere = ft_atoi_double(data[2]);
	if(scene->sphere->diameter_sphere < 0)
		exit_error("invalid diametre", "in sphere");
	char **colors = ft_split(data[3], ',');
	if(!check_colors_args(colors))
		exit_error("some color is missing", "in SP");
	scene->sphere->color_sphere->r = ft_atoi_color(colors[0], "sphere");
	scene->sphere->color_sphere->g = ft_atoi_color(colors[1], "sphere");
	scene->sphere->color_sphere->b = ft_atoi_color(colors[2], "sphere");
}
void	ft_cylinder(char **data, t_scene *scene)
{
	int i;
	for(i = 0; data[i]; i++)
		i++;
	if(i != 6)
		exit_error("invalid arguments", "in CY");
	char **coors = ft_split(data[1], ',');
	scene->cylinder->coor_cylinder->x = ft_atoi_double(coors[0]);
	scene->cylinder->coor_cylinder->y = ft_atoi_double(coors[1]);
	scene->cylinder->coor_cylinder->z = ft_atoi_double(coors[2]);

	char **vects = ft_split(data[2], ',');
	scene->cylinder->vector_cylinder->x = ft_atoi_double(vects[0]);
	scene->cylinder->vector_cylinder->y = ft_atoi_double(vects[1]);
	scene->cylinder->vector_cylinder->z = ft_atoi_double(vects[2]);

	double range_x = scene->cylinder->vector_cylinder->x;
	double range_y = scene->cylinder->vector_cylinder->y;
	double range_z = scene->cylinder->vector_cylinder->z;
	if(range_x < -1 || range_x > 1 || range_y < -1 || range_y > 1 || range_z < -1 || range_z > 1)
			exit_error("invalid range vectors", "CY");

	scene->cylinder->diameter_cylinder = ft_atoi_double(data[3]);
	
	scene->cylinder->height_cylinder = ft_atoi_double(data[4]);

	char **colors = ft_split(data[5], ',');
	if(!check_colors_args(colors))
		exit_error("some color is missing", "in CY");
	scene->cylinder->color_cylinder->r = ft_atoi_color(colors[0], "cylinder");
	scene->cylinder->color_cylinder->g = ft_atoi_color(colors[1], "cylinder");
	scene->cylinder->color_cylinder->b = ft_atoi_color(colors[2], "cylinder");
}

