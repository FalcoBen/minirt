#include "minirt.h"

void	init_objects(t_objects *input_data, char ***tokens ,int counter)
{
	int i = 0;
	int j = 0;
	while(tokens[i] && i < counter)
	{
		if(strcmp(tokens[i][0], "A") == 0)
		{
			input_data[j].identifier = "A";
			input_data[j].assign_object = ft_ambient_light;
		}
		else if(strcmp(tokens[i][0], "C") == 0)
		{
			input_data[j].identifier = "C";
			input_data[j].assign_object = ft_camera;
		}
		else if(strcmp(tokens[i][0], "L") == 0)
		{
			input_data[j].identifier = "L";
			input_data[j].assign_object = ft_light;
		}
		else if(strcmp(tokens[i][0], "pl") == 0)
		{
			input_data[j].identifier = "pl";
			input_data[j].assign_object = ft_plane;
		}
		else if(strcmp(tokens[i][0], "sp") == 0)
		{
			input_data[j].identifier = "sp";
			input_data[j].assign_object = ft_sphere;
		}
		else if(strcmp(tokens[i][0], "cy") == 0)
		{
			input_data[j].identifier = "cy";
			input_data[j].assign_object = ft_cylinder;
		}
		i++;
		j++;
	}
	input_data[j].identifier = NULL;
	input_data[j].assign_object = NULL;
}

void	ft_ambient_light(char **data, t_scene *scene)
{
	scene->ambient_light->bright_ambient_light = ft_atoi_double(data[1]);
	if(scene->ambient_light->bright_ambient_light <= 0 || scene->ambient_light->bright_ambient_light >= 1)
		exit_error("invalid range ambient light", "A");
	char **colors = ft_split(data[2], ',');
	scene->ambient_light->color_ambient_light->r = ft_atoi_color(colors[0]);
	scene->ambient_light->color_ambient_light->g = ft_atoi_color(colors[1]);
	scene->ambient_light->color_ambient_light->b = ft_atoi_color(colors[2]);
}
void	ft_camera(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->camera->coor_camera->x = ft_atoi_double(coors[0]);
	scene->camera->coor_camera->y = ft_atoi_double(coors[1]);
	scene->camera->coor_camera->z = ft_atoi_double(coors[2]);
	char **vects = ft_split(data[2], ',');
	scene->camera->vector_camera->x = ft_atoi_double(vects[0]);
	scene->camera->vector_camera->y = ft_atoi_double(vects[1]);
	scene->camera->vector_camera->z = ft_atoi_double(vects[2]);
	double range_x = scene->camera->vector_camera->x;
	double range_y = scene->camera->vector_camera->x;
	double range_z = scene->camera->vector_camera->x;
	if(range_x <= -1 || range_x >= 1 || range_y <= -1 || range_y >= 1 || range_z <= -1 || range_z >= 1)
		exit_error("invalid range vectors", "C");
	scene->camera->angle_view = ft_atoi_double(data[3]);
	if(scene->camera->angle_view <= 0 || scene->camera->angle_view <= 180)
		exit_error("invalid range angle view", "C");

}
void	ft_light(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->light->coor_light->x = ft_atoi_double(coors[0]);
	scene->light->coor_light->y = ft_atoi_double(coors[1]);
	scene->light->coor_light->z = ft_atoi_double(coors[2]);

	scene->light->bright_light = ft_atoi_double(data[2]);
	if(scene->light->bright_light <= 0 || scene->light->bright_light >= 1)
		exit_error("invalid range bright light", "L");
	char **colors = ft_split(data[3], ',');
	scene->light->color_light->r = ft_atoi_color(colors[0]);
	scene->light->color_light->g = ft_atoi_color(colors[1]);
	scene->light->color_light->b = ft_atoi_color(colors[2]);
}
void	ft_plane(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->plane->coor_plane->x = ft_atoi_double(coors[0]);
	scene->plane->coor_plane->y = ft_atoi_double(coors[1]);
	scene->plane->coor_plane->z = ft_atoi_double(coors[2]);

	char **vects = ft_split(data[2], ',');
	scene->plane->vector_plane->x = ft_atoi_double(vects[0]);
	scene->plane->vector_plane->y = ft_atoi_double(vects[1]);
	scene->plane->vector_plane->z = ft_atoi_double(vects[2]);

	double range_x = scene->camera->vector_camera->x;
	double range_y = scene->camera->vector_camera->x;
	double range_z = scene->camera->vector_camera->x;
	if(range_x <= -1 || range_x >= 1 || range_y <= -1 || range_y >= 1 || range_z <= -1 || range_z >= 1)
			exit_error("invalid range vectors", "PL");

	char **colors = ft_split(data[3], ',');
	scene->plane->color_plane->r = ft_atoi_color(colors[0]);
	scene->plane->color_plane->g = ft_atoi_color(colors[1]);
	scene->plane->color_plane->b = ft_atoi_color(colors[2]);

}
void	ft_sphere(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->sphere->coor_sphere->x = ft_atoi_double(coors[0]);
	scene->sphere->coor_sphere->y = ft_atoi_double(coors[1]);
	scene->sphere->coor_sphere->z = ft_atoi_double(coors[2]);

	scene->sphere->diameter_sphere = ft_atoi_double(data[2]);

	char **colors = ft_split(data[3], ',');
	scene->sphere->color_sphere->r = ft_atoi_color(colors[0]);
	scene->sphere->color_sphere->g = ft_atoi_color(colors[1]);
	scene->sphere->color_sphere->b = ft_atoi_color(colors[2]);
}
void	ft_cylinder(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->cylinder->coor_cylinder->x = ft_atoi_double(coors[0]);
	scene->cylinder->coor_cylinder->y = ft_atoi_double(coors[1]);
	scene->cylinder->coor_cylinder->z = ft_atoi_double(coors[2]);

	char **vects = ft_split(data[2], ',');
	scene->cylinder->vector_cylinder->x = ft_atoi_double(vects[0]);
	scene->cylinder->vector_cylinder->y = ft_atoi_double(vects[1]);
	scene->cylinder->vector_cylinder->z = ft_atoi_double(vects[2]);

	double range_x = scene->camera->vector_camera->x;
	double range_y = scene->camera->vector_camera->x;
	double range_z = scene->camera->vector_camera->x;
	if(range_x <= -1 || range_x >= 1 || range_y <= -1 || range_y >= 1 || range_z <= -1 || range_z >= 1)
			exit_error("invalid range vectors", "CY");

	scene->cylinder->diameter_cylinder = ft_atoi_double(data[3]);
	
	scene->cylinder->height_cylinder = ft_atoi_double(data[4]);

	char **colors = ft_split(data[5], ',');
	scene->cylinder->color_cylinder->r = ft_atoi_color(colors[0]);
	scene->cylinder->color_cylinder->g = ft_atoi_color(colors[1]);
	scene->cylinder->color_cylinder->b = ft_atoi_color(colors[2]);
}

