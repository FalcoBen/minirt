#include "parsing.h"

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
	// free(new_node->identifier);
	// ft_lstclear((void **)head, del, 'o');
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


void	ft_ambient_light_fb(char **data, t_scene *scene)
{
	int				i;
	t_ambient_light_fb	*new_ambient;
	t_ambient_light_fb	*current;
	char			**colors;

	i = 0;
	while (data[i])
		i++;
	if (i != 3)
		exit_error("invalid arguments", "in A", scene->cleaner);

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
	ft_free_split(colors);
	
}


void	ft_camera_fb(char **data, t_scene *scene)
{
	int i = 0;
	while(data[i])
		i++;
	if(i != 4)
		exit_error("invalid arguments", "in C", scene->cleaner);

	t_camera_fb *new_camera = malloc(sizeof(t_camera_fb));
	t_camera_fb *current;
	new_camera->coor_camera = malloc(sizeof(t_vec3));
	new_camera->next = NULL;
	char **coors = ft_split(data[1], ',');
	new_camera->coor_camera->x = ft_atoi_double(coors[0], scene->cleaner);
	new_camera->coor_camera->y = ft_atoi_double(coors[1], scene->cleaner);
	new_camera->coor_camera->z = ft_atoi_double(coors[2], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in coors", "C", scene->cleaner);
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
	new_camera->angle_view = ft_atoi_double(data[3], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in angle", "C", scene->cleaner);
	if(new_camera->angle_view < 0 || new_camera->angle_view > 180)
		exit_error("invalid range angle view", "C", scene->cleaner);
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
	ft_free_split(coors);
	ft_free_split(vects);
}
void	ft_light(char **data, t_scene *scene)
{
	t_light_fb *new_light;
	t_light_fb *current;

	int i = 0;
	while (data[i])
		i++;
	if(i != 4)
		exit_error("invalid arguments", "in L", scene->cleaner);
		
		new_light = malloc(sizeof(t_light_fb));
		new_light->color_light = malloc(sizeof(t_color_fb));
		new_light->coor_light = malloc(sizeof(t_vec3));
	new_light->next = NULL;
	
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
	char **colors = ft_split(data[3], ',');
	// printf("----|%s|\n", colors[2]);
	new_light->color_light->r = ft_atoi_color(colors[0], "light");
	new_light->color_light->g = ft_atoi_color(colors[1], "light");
	new_light->color_light->b = ft_atoi_color(colors[2], "light");
	if(new_light->color_light->r == -1 || new_light->color_light->g == -1 ||new_light->color_light->b == -1)
		exit_error("somthing wrong with colors", "in light", scene->cleaner);
	
	if(scene->light == NULL)
	scene->light = new_light;
	else
	{
		current = scene->light;
		while(current->next)
		current = current->next;
		current->next = new_light;
	}
	ft_free_split(colors);
	ft_free_split(coors);
}
void	ft_plane_fb(char **data, t_scene *scene)
{
	t_plane_fb *new_plane;
	t_plane_fb *current;
	
	int i = 0;
	while(data[i] != NULL)
		i++;
	i--;
	if(!verify_data_plane(data))
	{
		exit_error("data not in the correct format", "in plane", scene->cleaner);
	}
	scene->type = T_PLAN;
	new_plane = malloc(sizeof(t_plane_fb));
	new_plane->flag_bump = false;
	if (i == 4 || i == 5)
	{
		int type_bump = ft_atoi_color(data[4], "bump plane");
		if(type_bump == 1 || type_bump == 2)
		{
			if(type_bump == 1)
			{
				if(i == 5)
					exit_error("nothing should be after this type of bump", "in plane", scene->cleaner);
				new_plane->flag_bump = true;
				t_texture *test_bump_checker = malloc(sizeof(t_texture));
				test_bump_checker->type = 1;
				// test_bump_checker->color_solid = (t_color_fb){0,0,0};
				// test_bump_checker->color_checkerd = (t_color_fb){255,255,255};
				test_bump_checker->scale = 0.1;
				test_bump_checker->image = NULL;
				new_plane->img_path = NULL;
				new_plane->bump_texture = test_bump_checker;		
			}
			else if(type_bump == 2)
			{
				if(i == 5)
				{
					new_plane->flag_bump = true;
					new_plane->img_path = ft_strdup(data[5]);
					t_texture *test_bump = malloc(sizeof(t_texture));
					test_bump->type = 2;
					test_bump->color_solid = (t_color_fb){0,0,0};
					test_bump->color_checkerd = (t_color_fb){0,0,0};
					test_bump->scale = 0.002;  // bump strength
					test_bump->image = mlx_load_png(new_plane->img_path);
					if (!test_bump->image)
					{
						free(new_plane->img_path);
						free(test_bump);
						free(new_plane);
						perror("png");
						exit_error("incorrect image path", "in plane", scene->cleaner);
					}
					else
					{
						new_plane->bump_texture = test_bump;
					}
				}
				else
				{
					free(new_plane);
					exit_error("need a png", "in plane", scene->cleaner);
				}
			}
		}
		else
		{
			free(new_plane);
			exit_error("malformat in type of bump ", "in plane", scene->cleaner);	
		}
	}

	new_plane->color_plane = malloc(sizeof(t_color_fb));
	new_plane->coor_plane = malloc(sizeof(t_vec3));
	new_plane->vector_plane = malloc(sizeof(t_vec3));
	new_plane->next = NULL;

	char **coors = ft_split(data[1], ',');
	new_plane->coor_plane->x = ft_atoi_double(coors[0], scene->cleaner);
	new_plane->coor_plane->y = ft_atoi_double(coors[1], scene->cleaner);
	new_plane->coor_plane->z = ft_atoi_double(coors[2], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "pl", scene->cleaner);

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
	char **colors = ft_split(data[3], ',');	
	new_plane->color_plane->r = ft_atoi_color(colors[0], "plane");
	new_plane->color_plane->g = ft_atoi_color(colors[1], "plane");
	new_plane->color_plane->b = ft_atoi_color(colors[2], "plane");
	if(new_plane->color_plane->r == -1 || new_plane->color_plane->g == -1 ||new_plane->color_plane->b == -1)
		exit_error("somthing wrong with colors", "in plane", scene->cleaner);
	
	if(scene->plane == NULL)
		scene->plane = new_plane;
	else
	{
		current = scene->plane;
		while(current->next)
			current = current->next;
		current->next = new_plane;
	}

	ft_free_split(colors);
	ft_free_split(coors);
	ft_free_split(vects);
}


void	ft_sphere_fb(char **data, t_scene *scene)
{
	t_sphere_fb *new_sphere;
	t_sphere_fb  *current;
	if(!verify_data_sphere(data, scene->cleaner))
	{
		exit_error("data not in the correct format", "in sphere", scene->cleaner);
	}
	new_sphere = malloc(sizeof(t_sphere_fb));

	new_sphere->next = NULL;
	new_sphere->bump_texture = NULL;
	new_sphere->img_path = NULL;
	new_sphere->flag_bump = false;
	int i = 0;
	while(data[i] != NULL)
	{
		i++;
	}
	i--;
	scene->type = T_SPHERE;
	if (i == 4 || i == 5)
	{
		int type_bump = ft_atoi_color(data[4], "bump in sphere");
		if(type_bump == 1 || type_bump == 2)
		{
			if(type_bump == 1)
			{
				if(i == 5)
					exit_error("nothing should be after this type of bump", "in sphere", scene->cleaner);
				new_sphere->flag_bump = true;
				t_texture *test_bump_checker = malloc(sizeof(t_texture));
				test_bump_checker->type = 1;
				test_bump_checker->color_solid = (t_color_fb){0,0,0};
				test_bump_checker->color_checkerd = (t_color_fb){255,255,255};
				test_bump_checker->scale = 0.1;
				test_bump_checker->image = NULL;	
				new_sphere->bump_texture = test_bump_checker;	
			}
			else if(type_bump == 2)
			{
				if(i == 5)
				{
					new_sphere->flag_bump = true;
					new_sphere->img_path = ft_strdup(data[5]);
					t_texture *test_bump = malloc(sizeof(t_texture));
					test_bump->type = 2;
					test_bump->color_solid = (t_color_fb){0,0,0};
					test_bump->color_checkerd = (t_color_fb){0,0,0};
					test_bump->scale = 0.1;  // bump strength
					test_bump->image = mlx_load_png(new_sphere->img_path);
					if (!test_bump->image)
					{
						free(new_sphere->img_path);
						free(test_bump);
						free(new_sphere);
						perror("png");
						exit_error("incorrect image path", "in plane", scene->cleaner);
					}
					else
					{
						new_sphere->bump_texture = test_bump;
					}
				}
				else
				{
					free(new_sphere);
					exit_error("need a png", "in sphere", scene->cleaner);
				}
			}
		}
		else
		{

			free(new_sphere);
			exit_error("malformat in type of bump", "in sphere", scene->cleaner);

		}

	}

	new_sphere->color_sphere = malloc(sizeof(t_color_fb));
	new_sphere->coor_sphere = malloc(sizeof(t_vec3));
	new_sphere->next = NULL;

	char **coors = ft_split(data[1], ',');
	new_sphere->coor_sphere->x = ft_atoi_double(coors[0], scene->cleaner);
	new_sphere->coor_sphere->y = ft_atoi_double(coors[1], scene->cleaner);
	new_sphere->coor_sphere->z = ft_atoi_double(coors[2], scene->cleaner);
	// new_sphere->coor_sphere->w = ft_atoi_double("0.1");
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "sp", scene->cleaner);

	new_sphere->diameter_sphere = ft_atoi_double(data[2], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in diameter", "sp", scene->cleaner);
	
	if(new_sphere->diameter_sphere <= 0)
		exit_error("invalid diametre", "in sphere", scene->cleaner);
	
	if(count_comma(data[3]) != 2)
		exit_error("more or less in color", "in spher", scene->cleaner);
	char **colors = ft_split(data[3], ',');
	new_sphere->color_sphere->r = ft_atoi_color(colors[0], "sphere");
	new_sphere->color_sphere->g = ft_atoi_color(colors[1], "sphere");
	new_sphere->color_sphere->b = ft_atoi_color(colors[2], "sphere");
	if(new_sphere->color_sphere->r == -1 || new_sphere->color_sphere->g == -1 ||new_sphere->color_sphere->b == -1)
		exit_error("somthing wrong with colors", "in sphere", scene->cleaner);
		
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
	ft_free_split(colors);
	ft_free_split(coors);
}
void	ft_cylinder_fb(char **data, t_scene *scene)
{
	printf("in cylinder ----------------------------------------------\n");
	t_cylinder_fb *new_cylinder;
	t_cylinder_fb *current;
	if(!verify_data_cylinder(data, scene->cleaner))
	{
		exit_error("data not in the correct format", "in cylinder", scene->cleaner);
	}
	int i = 0;
	while(data[i])
		i++;
	i--;
	new_cylinder = malloc(sizeof(t_cylinder_fb));
	new_cylinder->flag_bump = false;
	new_cylinder->closed = false;
	new_cylinder->bump_texture = NULL;
	new_cylinder->img_path = NULL;
	scene->type = T_CYLINDRE;

	if (i == 7 || i == 8) 
	{
		int type_bump = ft_atoi_color(data[7], "bump in cylinder");
		if(type_bump == 1 || type_bump == 2)
		{
			if(type_bump == 1)
			{
				if(i != 7 )
					exit_error("nothing should be after this type of bump", "in cylinder", scene->cleaner);
				new_cylinder->flag_bump = true;
				t_texture *test_bump_checker = malloc(sizeof(t_texture));
				test_bump_checker->type = 1;
				test_bump_checker->scale = 0.1;
				test_bump_checker->image = NULL;	
				new_cylinder->bump_texture = test_bump_checker;	
			}
			else if(type_bump == 2)
			{
				if(i == 8)
				{
					new_cylinder->flag_bump = true;
					new_cylinder->img_path = ft_strdup(data[8]);
					t_texture *test_bump = malloc(sizeof(t_texture));
					test_bump->type = 2;
					test_bump->color_solid = (t_color_fb){0,0,0};
					test_bump->color_checkerd = (t_color_fb){0,0,0};
					// test_bump->scale = 0.1;
					test_bump->image = mlx_load_png(new_cylinder->img_path);
					if (!test_bump->image)
					{
						free(new_cylinder->img_path);
						free(test_bump);
						free(new_cylinder);
						perror("png");
						exit_error("incorrect image path", "in cylinder", scene->cleaner);
					}
					else
					{
						new_cylinder->bump_texture = test_bump;
					}
				}
				else
				{
					free(new_cylinder);
					exit_error("need a png", "in cylinder", scene->cleaner);
				}
			}
		}
		else
		{
			free(new_cylinder);
			exit_error("malformat in type of bump", "in cylinder", scene->cleaner);
		}
	}
	
	new_cylinder->color_cylinder = malloc(sizeof(t_color_fb));
	new_cylinder->coor_cylinder = malloc(sizeof(t_vec3));
	new_cylinder->vector_cylinder = malloc(sizeof(t_vec3));
	new_cylinder->next = NULL;
	char **coors = ft_split(data[1], ',');
	new_cylinder->coor_cylinder->x = ft_atoi_double(coors[0], scene->cleaner);
	new_cylinder->coor_cylinder->y = ft_atoi_double(coors[1], scene->cleaner);
	new_cylinder->coor_cylinder->z = ft_atoi_double(coors[2], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "cy", scene->cleaner);
	char **vects = ft_split(data[2], ',');
	new_cylinder->vector_cylinder->x = ft_atoi_double(vects[0], scene->cleaner);
	new_cylinder->vector_cylinder->y = ft_atoi_double(vects[1], scene->cleaner);
	new_cylinder->vector_cylinder->z = ft_atoi_double(vects[2], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in vectors", "cy", scene->cleaner);
	double range_x = new_cylinder->vector_cylinder->x;
	double range_y = new_cylinder->vector_cylinder->y;
	double range_z = new_cylinder->vector_cylinder->z;
	if(range_x < -1 || range_x > 1 || range_y < -1 || range_y > 1 || range_z < -1 || range_z > 1)
		exit_error("invalid range vectors", "CY", scene->cleaner);
	
	new_cylinder->diameter_cylinder = ft_atoi_double(data[3], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in diameter_cylinder", "cy", scene->cleaner);
	new_cylinder->height_cylinder_fb = ft_atoi_double(data[4], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in hight cylinder", "cy", scene->cleaner);
	if (new_cylinder->height_cylinder_fb <= 0 || new_cylinder->diameter_cylinder <= 0)
		exit_error("invalid range in height or diameter", "CY", scene->cleaner);
	int open_or_close = atoi(data[5]);
	if(open_or_close == 0)
		new_cylinder->closed = true;
	else if (open_or_close == -1)
		new_cylinder->closed = false;
	else
		exit_error("flag for closed or open required to be 0 or -1", "in Cylinder", scene->cleaner);
	char **colors = ft_split(data[6], ',');
	new_cylinder->color_cylinder->r = ft_atoi_color(colors[0], "cylinder");
	new_cylinder->color_cylinder->g = ft_atoi_color(colors[1], "cylinder");
	new_cylinder->color_cylinder->b = ft_atoi_color(colors[2], "cylinder");
	if(scene->cylinder == NULL)
		scene->cylinder = new_cylinder;
	else
	{
		current = scene->cylinder;
		while(current->next)
			current = current->next;
		current->next = new_cylinder;
	}
	ft_free_split(colors);
	ft_free_split(coors);
	ft_free_split(vects);
}



/*----------------------------------------------------------------*/

void	ft_cone_fb(char **data, t_scene *scene)
{
	printf("------------------constructor cone -------------------------\n");
	t_cone_fb *new_cone;
	t_cone_fb *current;
	if(!verify_data_cone(data, scene->cleaner))
	{
		exit_error("data not in the correct format", "in cone", scene->cleaner);
	}
	int i = 0;
	while(data[i])
		i++;
	i--;
	new_cone = malloc(sizeof(t_cone_fb));
	new_cone->flag_bump = false;
	new_cone->bump_texture = NULL;
	new_cone->img_path = NULL;
	new_cone->closed_flag = false;
	scene->type = T_CONE;

	if (i == 8 || i == 9) 
	{
		puts("**********************\n");
		int type_bump = ft_atoi_color(data[8], "bump in cone");
		if(type_bump == 1 || type_bump == 2)
		{
			if(type_bump == 1)
			{
				if(i == 7)
					exit_error("nothing should be after this type of bump", "in cone", scene->cleaner);
				new_cone->flag_bump = true;
				t_texture *test_bump_checker = malloc(sizeof(t_texture));
				test_bump_checker->type = 1;
				test_bump_checker->scale = 0.1;
				test_bump_checker->image = NULL;	
				new_cone->bump_texture = test_bump_checker;	
			}
			else if(type_bump == 2)
			{
				if(i == 9)
				{
					new_cone->flag_bump = true;
					new_cone->img_path = ft_strdup(data[9]);
					t_texture *test_bump = malloc(sizeof(t_texture));
					test_bump->type = 2;
					test_bump->color_solid = (t_color_fb){0,0,0};
					test_bump->color_checkerd = (t_color_fb){0,0,0};
					// test_bump->scale = 0.1;
					test_bump->image = mlx_load_png(new_cone->img_path);
					if (!test_bump->image)
					{
						free(new_cone->img_path);
						free(test_bump);
						free(new_cone);
						perror("png");
						exit_error("incorrect image path", "in cone", scene->cleaner);
					}
					else
					{
						new_cone->bump_texture = test_bump;
					}
				}
				else
				{
					free(new_cone);
					exit_error("need a png", "in cone", scene->cleaner);
				}
			}
		}
		else
		{
			free(new_cone);
			exit_error("malformat in type of bump", "in cone", scene->cleaner);
		}
	}
	
	new_cone->color_cone = malloc(sizeof(t_color_fb));
	new_cone->coor_cone = malloc(sizeof(t_vec3));
	new_cone->vector_cone = malloc(sizeof(t_vec3));
	new_cone->next = NULL;
	char **coors = ft_split(data[1], ',');
	new_cone->coor_cone->x = ft_atoi_double(coors[0], scene->cleaner);
	new_cone->coor_cone->y = ft_atoi_double(coors[1], scene->cleaner);
	new_cone->coor_cone->z = ft_atoi_double(coors[2], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in coor", "cone", scene->cleaner);
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
	
	new_cone->minimum = ft_atoi_double(data[3], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in minimum", "cone", scene->cleaner);
	new_cone->maximum = ft_atoi_double(data[4], scene->cleaner);
	if(scene->cleaner->flag_exit)
		exit_error("invalid thing in hight cone", "cone", scene->cleaner);
	// if (new_cone->maximum <= 0 || new_cone->minimum <= 0)
	// 	exit_error("invalid range in maximum/minimum", "cone", scene->cleaner);
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
	char **colors = ft_split(data[7], ',');
	new_cone->color_cone->r = ft_atoi_color(colors[0], "cone");
	new_cone->color_cone->g = ft_atoi_color(colors[1], "cone");
	new_cone->color_cone->b = ft_atoi_color(colors[2], "cone");
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
	ft_free_split(colors);
	ft_free_split(coors);
	ft_free_split(vects);
}
