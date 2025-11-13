#include "parsing.h"

void    objects_linking_constractor(char *identifier, void (*assign_object)(char **, t_scene *), t_objects_fb **head, t_objects_fb *current)
{
	t_objects_fb *new_node = malloc(sizeof(t_objects_fb));
    if (!new_node)
		return;

    new_node->identifier = ft_strdup(identifier);
    new_node->assign_object = assign_object;
    // new_node->nb = 1;
    new_node->next = NULL;
	// printf("----------------------> %s\n", identifier);
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
	if(i != 3)
		return false;
	return true;
}

void	check_valid_args(char **data, t_scene *scene, char object)
{
	int i = 0;
	i = 0;
	while (data[i])
		i++;
	if(object == 'A')
	{
		if (i != 3)
			exit_error("invalid arguments", "in A", scene->cleaner);
	}
	if(object == 'C')
	{
		if(i != 4)
			exit_error("invalid arguments", "in C", scene->cleaner);
	}
	if(object == 'L')
	{
		if(i != 4)
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
	if(counter > 1)
		exit_error("duplicate object", "ambient_light", scene->cleaner);
}
void	camera_linked_list(t_camera_fb *new_camera, t_scene *scene)
{
	t_camera_fb	*current;

	if(scene->camera == NULL)
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
	if(counter > 1)
		exit_error("duplicate object", "camera", scene->cleaner);
}
void	light_linked_list(t_light_fb *new_light, t_scene *scene)
{

	t_light_fb *current;
	if(scene->light == NULL)
		scene->light = new_light;
	else
	{
		current = scene->light;
		while(current->next)
			current = current->next;
		current->next = new_light;
	}
}
void	plane_linked_list(t_plane_fb *new_plane, t_scene *scene)
{
	t_plane_fb *current;

	if(scene->plane == NULL)
		scene->plane = new_plane;
	else
	{
		current = scene->plane;
		while(current->next)
			current = current->next;
		current->next = new_plane;
	}
}
void	sphere_linked_list(t_sphere_fb *new_sphere, t_scene *scene)
{
	t_sphere_fb *current;

	if(scene->sphere == NULL)
	{
		puts("in if spher\n");	
		scene->sphere = new_sphere;
	}
	else
	{
		current = scene->sphere;
		while(current->next)
			current = current->next;
		current->next = new_sphere;
	}
}
void	cylinder_linked_list(t_cylinder_fb *new_cylinder, t_scene *scene)
{
	t_cylinder_fb *current;
	if(scene->cylinder == NULL)
		scene->cylinder = new_cylinder;
	else
	{
		current = scene->cylinder;
		while(current->next)
			current = current->next;
		current->next = new_cylinder;
	}
}
void	cone_linked_list(t_cone_fb *new_cone, t_scene *scene)
{
	t_cone_fb *current;
	if(scene->cone == NULL)
	{
		scene->cone = new_cone;
	}
	else
	{
		current = scene->cone;
		while(current->next)
			current = current->next;
		current->next = new_cone;
	}
}
 
void	ft_ambient_light_fb(char **data, t_scene *scene)
{	
	t_ambient_light_fb	*new_ambient;
	char			**colors;
	
	check_valid_args(data, scene, 'A');
	new_ambient = malloc(sizeof(t_ambient_light_fb));
	new_ambient->color_ambient_light = malloc(sizeof(t_color_fb));
	new_ambient->next = NULL;

	new_ambient->bright_ambient_light_fb = ft_atoi_double(data[1], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in", "A", scene->cleaner);
	if (new_ambient->bright_ambient_light_fb < 0 || new_ambient->bright_ambient_light_fb > 1)
		exit_error("invalid range ambient light", "A", scene->cleaner);
	if(count_comma(data[2]) != 2)
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
	if(scene->cleaner->flag_exit)
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
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in vectors", "C", scene->cleaner);
	double range_x = new_camera->vector_camera->x;
	double range_y = new_camera->vector_camera->y;
	double range_z = new_camera->vector_camera->z;
	if(range_x < -1 || range_x > 1)
		exit_error("invalid range vectors", "C in x", scene->cleaner);

	if(range_y < -1 || range_y > 1)
		exit_error("invalid range vectors", "C in y", scene->cleaner);
		
	if(range_z < -1 || range_z > 1)
		exit_error("invalid range vectors", "C in z", scene->cleaner);
	ft_free_split(vects);
	
}
void	ft_camera_fb(char **data, t_scene *scene)
{
	check_valid_args(data, scene, 'C');

	t_camera_fb *new_camera = malloc(sizeof(t_camera_fb));
	t_camera_fb *current;
	new_camera->next = NULL;
	coor_camera(new_camera, data, scene);
	vector_camera(new_camera, data, scene);
	new_camera->angle_view = ft_atoi_double(data[3], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in angle", "C", scene->cleaner);
	if(new_camera->angle_view < 0 || new_camera->angle_view > 180)
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
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "L", scene->cleaner);
	new_light->bright_light = ft_atoi_double(data[2], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in bright_light", "L", scene->cleaner);
	if(new_light->bright_light < 0 || new_light->bright_light > 1)
		exit_error("invalid range bright light", "L", scene->cleaner);
	if(count_comma(data[3]) != 2)
		exit_error("more or less in color", "in light", scene->cleaner);
	ft_free_split(coors);
	
}
void	ft_light(char **data, t_scene *scene)
{
	t_light_fb *new_light;

	check_valid_args(data, scene, 'L');
	
	new_light = malloc(sizeof(t_light_fb));
	new_light->next = NULL;
	coor_light(new_light, data, scene);
	new_light->color_light = malloc(sizeof(t_color_fb));
	char **colors = ft_split(data[3], ',');
	new_light->color_light->r = ft_atoi_color(colors[0], "light");
	new_light->color_light->g = ft_atoi_color(colors[1], "light");
	new_light->color_light->b = ft_atoi_color(colors[2], "light");
	if(new_light->color_light->r == -1 || new_light->color_light->g == -1 ||new_light->color_light->b == -1)
		exit_error("somthing wrong with colors", "in light", scene->cleaner);
	
	light_linked_list(new_light, scene);
	ft_free_split(colors);
}

/*-------------------------------------*/
t_texture	*create_checker_bump(void)
{
    t_texture *test_bump_checker = malloc(sizeof(t_texture));
    test_bump_checker->type = 1;
    test_bump_checker->scale = 0.1;
    test_bump_checker->image = NULL;
    return (test_bump_checker);
}

t_texture	*create_image_bump(char *img_path, t_scene *scene)
{
    t_texture *test_bump = malloc(sizeof(t_texture));
    test_bump->type = 2;
    test_bump->color_solid = (t_color_fb){0, 0, 0};
    test_bump->color_checkerd = (t_color_fb){0, 0, 0};
    test_bump->scale = 0.002;
    test_bump->image = mlx_load_png(img_path);
    if (!test_bump->image)
    {
        free(img_path);
        free(test_bump);
        perror("png");
        exit_error("incorrect image path", "in plane", scene->cleaner);
    }
    return (test_bump);
}

void	bump_mapping_plane_type_1(t_plane_fb *new_plane, int i, t_scene *scene)
{
    if (i == 5)
        exit_error("nothing should be after this type of bump", "in plane", scene->cleaner);
    new_plane->img_path = NULL;
    new_plane->bump_texture = create_checker_bump();
}

void	bump_mapping_plane_type_2(t_plane_fb *new_plane, char **data, int i, t_scene *scene)
{
    if (i != 5)
    {
        free(new_plane);
        exit_error("need a png", "in plane", scene->cleaner);
    }
    new_plane->img_path = ft_strdup(data[5]);
    new_plane->bump_texture = create_image_bump(new_plane->img_path, scene);
}

void	bump_mapping_plane_constractor(t_plane_fb *new_plane, char **data, t_scene *scene, int i)
{
    int type_bump = ft_atoi_color(data[4], "bump plane");
    if (type_bump != 1 && type_bump != 2)
    {
        free(new_plane);
        exit_error("malformat in type of bump ", "in plane", scene->cleaner);
    }
    new_plane->flag_bump = true;
    if (type_bump == 1)
        bump_mapping_plane_type_1(new_plane, i, scene);
    else
        bump_mapping_plane_type_2(new_plane, data, i, scene);
}
/*--------------------------------------*/

void	coor_plane(t_plane_fb *new_plane, char **data, t_scene *scene)
{
	new_plane->coor_plane = malloc(sizeof(t_vec3));

	char **coors = ft_split(data[1], ',');
	new_plane->coor_plane->x = ft_atoi_double(coors[0], scene->cleaner);
	new_plane->coor_plane->y = ft_atoi_double(coors[1], scene->cleaner);
	new_plane->coor_plane->z = ft_atoi_double(coors[2], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "pl", scene->cleaner);
	ft_free_split(coors);
	
}
void	vects_plane(t_plane_fb *new_plane, char **data, t_scene *scene)
{
	new_plane->vector_plane = malloc(sizeof(t_vec3));

	char **vects = ft_split(data[2], ',');	
	new_plane->vector_plane->x = ft_atoi_double(vects[0], scene->cleaner);
	new_plane->vector_plane->y = ft_atoi_double(vects[1], scene->cleaner);
	new_plane->vector_plane->z = ft_atoi_double(vects[2], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in vectors", "pl", scene->cleaner);
	double range_x = new_plane->vector_plane->x;
	double range_y = new_plane->vector_plane->y;
	double range_z = new_plane->vector_plane->z;
	if(range_x < -1 || range_x > 1 || range_y < -1 || range_y > 1 || range_z < -1 || range_z > 1)
		exit_error("invalid range vectors", "PL", scene->cleaner);

	if(count_comma(data[3]) != 2)
		exit_error("more or less in color", "in plane", scene->cleaner);
	ft_free_split(vects);
	
}
void	color_plane(t_plane_fb *new_plane, char **data, t_scene *scene)
{
	new_plane->color_plane = malloc(sizeof(t_color_fb));

	char **colors = ft_split(data[3], ',');	
	new_plane->color_plane->r = ft_atoi_color(colors[0], "plane");
	new_plane->color_plane->g = ft_atoi_color(colors[1], "plane");
	new_plane->color_plane->b = ft_atoi_color(colors[2], "plane");
	if(new_plane->color_plane->r == -1 || new_plane->color_plane->g == -1 ||new_plane->color_plane->b == -1)
		exit_error("somthing wrong with colors", "in plane", scene->cleaner);
	ft_free_split(colors);
	
}
void	ft_plane_fb(char **data, t_scene *scene)
{
	t_plane_fb *new_plane;
	
	int i = 0;
	while(data[i] != NULL)
		i++;
	i--;
	if(!verify_data_plane(data))
		exit_error("data not in the correct format", "in plane", scene->cleaner);
	scene->type = T_PLAN;
	new_plane = malloc(sizeof(t_plane_fb));
	new_plane->flag_bump = false;
	if (i == 4 || i == 5)
		bump_mapping_plane_constractor(new_plane, data, scene, i);

	new_plane->next = NULL;
	coor_plane(new_plane, data, scene);
	vects_plane(new_plane, data, scene);
	color_plane(new_plane, data, scene);
	plane_linked_list(new_plane, scene);
}

/*--------------------------------------*/
t_texture	*create_checker_bump_sphere(void)
{
    t_texture *test_bump_checker = malloc(sizeof(t_texture));
    if (!test_bump_checker)
        return (NULL);
    test_bump_checker->type = 1;
    test_bump_checker->color_solid = (t_color_fb){0, 0, 0};
    test_bump_checker->color_checkerd = (t_color_fb){255, 255, 255};
    test_bump_checker->scale = 0.1;
    test_bump_checker->image = NULL;
    return (test_bump_checker);
}

t_texture	*create_image_bump_sphere(char *img_path, t_scene *scene)
{
    t_texture *test_bump = malloc(sizeof(t_texture));
    if (!test_bump)
        return (NULL);
    test_bump->type = 2;
    test_bump->color_solid = (t_color_fb){0, 0, 0};
    test_bump->color_checkerd = (t_color_fb){0, 0, 0};
    test_bump->scale = 0.1;
    test_bump->image = mlx_load_png(img_path);
    if (!test_bump->image)
    {
        free(img_path);
        free(test_bump);
        perror("png");
        exit_error("incorrect image path", "in sphere", scene->cleaner);
    }
    return (test_bump);
}

void	bump_mapping_sphere_type_1(t_sphere_fb *new_sphere, int i, t_scene *scene)
{
    if (i == 5)
        exit_error("nothing should be after this type of bump", "in sphere", scene->cleaner);
    new_sphere->img_path = NULL;
    new_sphere->bump_texture = create_checker_bump_sphere();
}

void	bump_mapping_sphere_type_2(t_sphere_fb *new_sphere, char **data, int i, t_scene *scene)
{
    if (i != 5)
    {
        free(new_sphere);
        exit_error("need a png", "in sphere", scene->cleaner);
    }
    new_sphere->img_path = ft_strdup(data[5]);
    new_sphere->bump_texture = create_image_bump_sphere(new_sphere->img_path, scene);
}

void	bump_mapping_sphere_constractor(t_sphere_fb *new_sphere, char **data, t_scene *scene, int i)
{
    int type_bump = ft_atoi_color(data[4], "bump in sphere");
    if (type_bump != 1 && type_bump != 2)
    {
        free(new_sphere);
        exit_error("malformat in type of bump", "in sphere", scene->cleaner);
    }
    new_sphere->flag_bump = true;
    if (type_bump == 1)
        bump_mapping_sphere_type_1(new_sphere, i, scene);
    else
        bump_mapping_sphere_type_2(new_sphere, data, i, scene);
}

/*--------------------------------------*/

void	coor_sphere(t_sphere_fb *new_sphere, char **data, t_scene *scene)
{
    new_sphere->coor_sphere = malloc(sizeof(t_vec3));
    char **coors = ft_split(data[1], ',');
    new_sphere->coor_sphere->x = ft_atoi_double(coors[0], scene->cleaner);
    new_sphere->coor_sphere->y = ft_atoi_double(coors[1], scene->cleaner);
    new_sphere->coor_sphere->z = ft_atoi_double(coors[2], scene->cleaner);
    if (scene->cleaner->flag_exit)
        exit_error("invalid thing in coor", "sp", scene->cleaner);
    ft_free_split(coors);
}

void	color_sphere(t_sphere_fb *new_sphere, char **data, t_scene *scene)
{
    new_sphere->color_sphere = malloc(sizeof(t_color_fb));
    if (count_comma(data[3]) != 2)
        exit_error("more or less in color", "in spher", scene->cleaner);
    char **colors = ft_split(data[3], ',');
    new_sphere->color_sphere->r = ft_atoi_color(colors[0], "sphere");
    new_sphere->color_sphere->g = ft_atoi_color(colors[1], "sphere");
    new_sphere->color_sphere->b = ft_atoi_color(colors[2], "sphere");
    if (new_sphere->color_sphere->r == -1 || new_sphere->color_sphere->g == -1 || new_sphere->color_sphere->b == -1)
        exit_error("somthing wrong with colors", "in sphere", scene->cleaner);
    ft_free_split(colors);
}

void	ft_sphere_fb(char **data, t_scene *scene)
{
    t_sphere_fb *new_sphere;

    if (!verify_data_sphere(data, scene->cleaner))
        exit_error("data not in the correct format", "in sphere", scene->cleaner);

    new_sphere = malloc(sizeof(t_sphere_fb));
    new_sphere->next = NULL;
    new_sphere->bump_texture = NULL;
    new_sphere->img_path = NULL;
    new_sphere->flag_bump = false;

    int i = 0;
    while (data[i] != NULL)
        i++;
    i--;
    scene->type = T_SPHERE;
    if (i == 4 || i == 5)
        bump_mapping_sphere_constractor(new_sphere, data, scene, i);
    coor_sphere(new_sphere, data, scene);
    new_sphere->diameter_sphere = ft_atoi_double(data[2], scene->cleaner);
    if (scene->cleaner->flag_exit)
        exit_error("invalid thing in diameter", "sp", scene->cleaner);
    if (new_sphere->diameter_sphere <= 0)
        exit_error("invalid diametre", "in sphere", scene->cleaner);
    color_sphere(new_sphere, data, scene);
    sphere_linked_list(new_sphere, scene);
}
t_texture	*create_checker_bump_cylinder(void)
{
    t_texture *test_bump_checker = malloc(sizeof(t_texture));
    if (!test_bump_checker)
        return (NULL);
    test_bump_checker->type = 1;
    test_bump_checker->scale = 0.1;
    test_bump_checker->image = NULL;
    return (test_bump_checker);
}

t_texture	*create_image_bump_cylinder(char *img_path, t_scene *scene)
{
    t_texture *test_bump = malloc(sizeof(t_texture));
    if (!test_bump)
        return (NULL);
    test_bump->type = 2;
    test_bump->color_solid = (t_color_fb){0, 0, 0};
    test_bump->color_checkerd = (t_color_fb){0, 0, 0};
    test_bump->scale = 0.1;
    test_bump->image = mlx_load_png(img_path);
    if (!test_bump->image)
    {
        free(img_path);
        free(test_bump);
        perror("png");
        exit_error("incorrect image path", "in cylinder", scene->cleaner);
    }
    return (test_bump);
}

void	bump_mapping_cylinder_type_1(t_cylinder_fb *new_cylinder, int i, t_scene *scene)
{
    if (i != 7)
        exit_error("nothing should be after this type of bump", "in cylinder", scene->cleaner);
    new_cylinder->img_path = NULL;
    new_cylinder->bump_texture = create_checker_bump_cylinder();
}

void	bump_mapping_cylinder_type_2(t_cylinder_fb *new_cylinder, char **data, int i, t_scene *scene)
{
    if (i != 8)
    {
        free(new_cylinder);
        exit_error("need a png", "in cylinder", scene->cleaner);
    }
    new_cylinder->img_path = ft_strdup(data[8]);
    new_cylinder->bump_texture = create_image_bump_cylinder(new_cylinder->img_path, scene);
}

void	bump_mapping_cylinder_constractor(t_cylinder_fb *new_cylinder, char **data, t_scene *scene, int i)
{
    int type_bump = ft_atoi_color(data[7], "bump in cylinder");
    if (type_bump != 1 && type_bump != 2)
    {
        free(new_cylinder);
        exit_error("malformat in type of bump", "in cylinder", scene->cleaner);
    }
    new_cylinder->flag_bump = true;
    if (type_bump == 1)
        bump_mapping_cylinder_type_1(new_cylinder, i, scene);
    else
        bump_mapping_cylinder_type_2(new_cylinder, data, i, scene);
}

void	coor_cylinder(t_cylinder_fb *new_cylinder, char **data, t_scene *scene)
{
    new_cylinder->coor_cylinder = malloc(sizeof(t_vec3));
    char **coors = ft_split(data[1], ',');
    new_cylinder->coor_cylinder->x = ft_atoi_double(coors[0], scene->cleaner);
    new_cylinder->coor_cylinder->y = ft_atoi_double(coors[1], scene->cleaner);
    new_cylinder->coor_cylinder->z = ft_atoi_double(coors[2], scene->cleaner);
    if (scene->cleaner->flag_exit)
        exit_error("invalid thing in coor", "cy", scene->cleaner);
    ft_free_split(coors);
}

void	vects_cylinder(t_cylinder_fb *new_cylinder, char **data, t_scene *scene)
{
    new_cylinder->vector_cylinder = malloc(sizeof(t_vec3));
    char **vects = ft_split(data[2], ',');
    new_cylinder->vector_cylinder->x = ft_atoi_double(vects[0], scene->cleaner);
    new_cylinder->vector_cylinder->y = ft_atoi_double(vects[1], scene->cleaner);
    new_cylinder->vector_cylinder->z = ft_atoi_double(vects[2], scene->cleaner);
    if (scene->cleaner->flag_exit)
        exit_error("invalid thing in vectors", "cy", scene->cleaner);
    double range_x = new_cylinder->vector_cylinder->x;
    double range_y = new_cylinder->vector_cylinder->y;
    double range_z = new_cylinder->vector_cylinder->z;
    if (range_x < -1 || range_x > 1 || range_y < -1 || range_y > 1 || range_z < -1 || range_z > 1)
        exit_error("invalid range vectors", "CY", scene->cleaner);
    ft_free_split(vects);
}

void	color_cylinder(t_cylinder_fb *new_cylinder, char **data, t_scene *scene)
{
    new_cylinder->color_cylinder = malloc(sizeof(t_color_fb));
    char **colors = ft_split(data[6], ',');
    new_cylinder->color_cylinder->r = ft_atoi_color(colors[0], "cylinder");
    new_cylinder->color_cylinder->g = ft_atoi_color(colors[1], "cylinder");
    new_cylinder->color_cylinder->b = ft_atoi_color(colors[2], "cylinder");
    ft_free_split(colors);
}

void	ft_cylinder_fb(char **data, t_scene *scene)
{
    t_cylinder_fb *new_cylinder;

    if (!verify_data_cylinder(data, scene->cleaner))
        exit_error("data not in the correct format", "in cylinder", scene->cleaner);

    int i = 0;
    while (data[i])
        i++;
    i--;
    new_cylinder = malloc(sizeof(t_cylinder_fb));
    new_cylinder->flag_bump = false;
    new_cylinder->closed = false;
    new_cylinder->bump_texture = NULL;
    new_cylinder->img_path = NULL;
    new_cylinder->next = NULL;
    scene->type = T_CYLINDRE;

    if (i == 7 || i == 8)
        bump_mapping_cylinder_constractor(new_cylinder, data, scene, i);

    coor_cylinder(new_cylinder, data, scene);
    vects_cylinder(new_cylinder, data, scene);

    new_cylinder->diameter_cylinder = ft_atoi_double(data[3], scene->cleaner);
    if (scene->cleaner->flag_exit)
        exit_error("invalid thing in diameter_cylinder", "cy", scene->cleaner);

    new_cylinder->height_cylinder_fb = ft_atoi_double(data[4], scene->cleaner);
    if (scene->cleaner->flag_exit)
        exit_error("invalid thing in hight cylinder", "cy", scene->cleaner);

    if (new_cylinder->height_cylinder_fb <= 0 || new_cylinder->diameter_cylinder <= 0)
        exit_error("invalid range in height or diameter", "CY", scene->cleaner);

    int open_or_close = atoi(data[5]);
    if (open_or_close == 0)
        new_cylinder->closed = true;
    else if (open_or_close == -1)
        new_cylinder->closed = false;
    else
        exit_error("flag for closed or open required to be 0 or -1", "in Cylinder", scene->cleaner);

    color_cylinder(new_cylinder, data, scene);
    cylinder_linked_list(new_cylinder, scene);
}

/*----------------------------------------------------------------*/
t_texture	*create_checker_bump_cone(void)
{
    t_texture *test_bump_checker = malloc(sizeof(t_texture));
    if (!test_bump_checker)
        return (NULL);
    test_bump_checker->type = 1;
    test_bump_checker->scale = 0.1;
    test_bump_checker->image = NULL;
    return (test_bump_checker);
}

t_texture	*create_image_bump_cone(char *img_path, t_scene *scene)
{
    t_texture *test_bump = malloc(sizeof(t_texture));
    if (!test_bump)
        return (NULL);
    test_bump->type = 2;
    test_bump->color_solid = (t_color_fb){0, 0, 0};
    test_bump->color_checkerd = (t_color_fb){0, 0, 0};
    test_bump->scale = 0.1;
    test_bump->image = mlx_load_png(img_path);
    if (!test_bump->image)
    {
        free(img_path);
        free(test_bump);
        perror("png");
        exit_error("incorrect image path", "in cone", scene->cleaner);
    }
    return (test_bump);
}

void	bump_mapping_cone_type_1(t_cone_fb *new_cone, int i, t_scene *scene)
{
    if (i != 8)
        exit_error("nothing should be after this type of bump", "in cone", scene->cleaner);
    new_cone->img_path = NULL;
    new_cone->bump_texture = create_checker_bump_cone();
}

void	bump_mapping_cone_type_2(t_cone_fb *new_cone, char **data, int i, t_scene *scene)
{
    if (i != 9)
    {
        free(new_cone);
        exit_error("need a png", "in cone", scene->cleaner);
    }
    new_cone->img_path = ft_strdup(data[9]);
    new_cone->bump_texture = create_image_bump_cone(new_cone->img_path, scene);
}

void	bump_mapping_cone_constractor(t_cone_fb *new_cone, char **data, t_scene *scene, int i)
{
    int type_bump = ft_atoi_color(data[8], "bump in cone");
    if (type_bump != 1 && type_bump != 2)
    {
        free(new_cone);
        exit_error("malformat in type of bump", "in cone", scene->cleaner);
    }
    new_cone->flag_bump = true;
    if (type_bump == 1)
        bump_mapping_cone_type_1(new_cone, i, scene);
    else
        bump_mapping_cone_type_2(new_cone, data, i, scene);
}


void	coor_cone(t_cone_fb *new_cone, char **data, t_scene *scene)
{
    new_cone->coor_cone = malloc(sizeof(t_vec3));
    char **coors = ft_split(data[1], ',');
    new_cone->coor_cone->x = ft_atoi_double(coors[0], scene->cleaner);
    new_cone->coor_cone->y = ft_atoi_double(coors[1], scene->cleaner);
    new_cone->coor_cone->z = ft_atoi_double(coors[2], scene->cleaner);
    if(scene->cleaner->flag_exit)
        exit_error("invalid thing in coor", "cone", scene->cleaner);
    ft_free_split(coors);
}

void	vects_cone(t_cone_fb *new_cone, char **data, t_scene *scene)
{
    new_cone->vector_cone = malloc(sizeof(t_vec3));
    char **vects = ft_split(data[2], ',');
    new_cone->vector_cone->x = ft_atoi_double(vects[0], scene->cleaner);
    new_cone->vector_cone->y = ft_atoi_double(vects[1], scene->cleaner);
    new_cone->vector_cone->z = ft_atoi_double(vects[2], scene->cleaner);
    if(scene->cleaner->flag_exit)
        exit_error("invalid thing in vectors", "cone", scene->cleaner);
    double range_x = new_cone->vector_cone->x;
    double range_y = new_cone->vector_cone->y;
    double range_z = new_cone->vector_cone->z;
    if(range_x < -1 || range_x > 1 || range_y < -1 || range_y > 1 || range_z < -1 || range_z > 1)
        exit_error("invalid range vectors", "cone", scene->cleaner);
    ft_free_split(vects);
}

void	color_cone(t_cone_fb *new_cone, char **data, t_scene *scene)
{
    new_cone->color_cone = malloc(sizeof(t_color_fb));
    char **colors = ft_split(data[7], ',');
    new_cone->color_cone->r = ft_atoi_color(colors[0], "cone");
    new_cone->color_cone->g = ft_atoi_color(colors[1], "cone");
    new_cone->color_cone->b = ft_atoi_color(colors[2], "cone");
    ft_free_split(colors);
}

void	ft_cone_fb(char **data, t_scene *scene)
{
    t_cone_fb *new_cone;

    if(!verify_data_cone(data, scene->cleaner))
        exit_error("data not in the correct format", "in cone", scene->cleaner);

    int i = 0;
    while(data[i])
        i++;
    i--;
    new_cone = malloc(sizeof(t_cone_fb));
    new_cone->flag_bump = false;
    new_cone->bump_texture = NULL;
    new_cone->img_path = NULL;
    new_cone->closed_flag = false;
    new_cone->next = NULL;
    scene->type = T_CONE;

    if (i == 8 || i == 9)
        bump_mapping_cone_constractor(new_cone, data, scene, i);

    coor_cone(new_cone, data, scene);
    vects_cone(new_cone, data, scene);

    new_cone->minimum = ft_atoi_double(data[3], scene->cleaner);
    if(scene->cleaner->flag_exit)
        exit_error("invalid thing in minimum", "cone", scene->cleaner);
    new_cone->maximum = ft_atoi_double(data[4], scene->cleaner);
    if(scene->cleaner->flag_exit)
        exit_error("invalid thing in hight cone", "cone", scene->cleaner);
    new_cone->angle = ft_atoi_double(data[5], scene->cleaner);
    if (new_cone->angle < 0 || new_cone->angle > 180)
        exit_error("invalid range inangle", "cone", scene->cleaner);

    int open_or_close = atoi(data[6]);
    if(open_or_close == 0)
        new_cone->closed_flag = true;
    else if (open_or_close == -1)
        new_cone->closed_flag = false;
    else
        exit_error("flag for closed or open required to be 0 or -1", "in Cylinder", scene->cleaner);

    color_cone(new_cone, data, scene);
    cone_linked_list(new_cone, scene);
}
