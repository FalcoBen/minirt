#include "minirt.h"

void	init_objects(t_objects *input_data)
{
	input_data[0].identifier = "A";
	input_data[0].assign_object = ft_ambient_light;
	input_data[1].identifier = "C";
	input_data[1].assign_object = ft_camera;
	input_data[2].identifier = "L";
	input_data[2].assign_object = ft_light;
	input_data[3].identifier = "pl";
	input_data[3].assign_object = ft_plane;
	input_data[4].identifier = "sp";
	input_data[4].assign_object = ft_sphere;
	input_data[5].identifier = "cy";
	input_data[5].assign_object = ft_cylinder;
	input_data[6].identifier = NULL;
	input_data[6].assign_object = NULL;
}

void	ft_ambient_light(char **data, t_scene *scene)
{
	scene->ambient_light->bright_ambient_light = ft_atoi_double(data[1]);

	char **colors = ft_split(data[2], ',');
	scene->ambient_light->color_ambient_light->r = ft_atoi(colors[0]);
	scene->ambient_light->color_ambient_light->g = ft_atoi(colors[1]);
	scene->ambient_light->color_ambient_light->b = ft_atoi(colors[2]);
}
void	ft_camera(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->camera->coor_camera.x = ft_atoi_double(coors[0]);
	scene->camera->coor_camera.y = ft_atoi_double(coors[1]);
	scene->camera->coor_camera.z = ft_atoi_double(coors[2]);

	char **vects = ft_split(data[2], ',');
	scene->camera->vector_camera.x = ft_atoi_double(vects[0]);
	scene->camera->vector_camera.y = ft_atoi_double(vects[1]);
	scene->camera->vector_camera.z = ft_atoi_double(vects[2]);
	scene->camera->angle_view = ft_atoi_double(data[3]);
}
void	ft_light(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->light->coor_light.x = ft_atoi_double(coors[0]);
	scene->light->coor_light.y = ft_atoi_double(coors[1]);
	scene->light->coor_light.z = ft_atoi_double(coors[2]);

	scene->light->bright_light = ft_atoi_double(data[2]);

	char **colors = ft_split(data[3], ',');
	scene->light->color_light->r = ft_atoi(colors[0]);
	scene->light->color_light->g = ft_atoi(colors[1]);
	scene->light->color_light->b = ft_atoi(colors[2]);
}
void	ft_plane(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->plane->coor_plane.x = ft_atoi_double(coors[0]);
	scene->plane->coor_plane.y = ft_atoi_double(coors[1]);
	scene->plane->coor_plane.z = ft_atoi_double(coors[2]);

	char **vects = ft_split(data[2], ',');
	scene->plane->vector_plane.x = ft_atoi_double(vects[0]);
	scene->plane->vector_plane.y = ft_atoi_double(vects[1]);
	scene->plane->vector_plane.z = ft_atoi_double(vects[2]);

	char **colors = ft_split(data[3], ',');
	scene->plane->color_plane->r = ft_atoi(colors[0]);
	scene->plane->color_plane->g = ft_atoi(colors[1]);
	scene->plane->color_plane->b = ft_atoi(colors[2]);

}
void	ft_sphere(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->sphere->coor_sphere.x = ft_atoi_double(coors[0]);
	scene->sphere->coor_sphere.y = ft_atoi_double(coors[1]);
	scene->sphere->coor_sphere.z = ft_atoi_double(coors[2]);

	scene->sphere->diameter_sphere = ft_atoi_double(data[2]);

	char **colors = ft_split(data[3], ',');
	scene->sphere->color_sphere->r = ft_atoi(colors[0]);
	scene->sphere->color_sphere->g = ft_atoi(colors[1]);
	scene->sphere->color_sphere->b = ft_atoi(colors[2]);
}
void	ft_cylinder(char **data, t_scene *scene)
{
	char **coors = ft_split(data[1], ',');
	scene->cylinder->coor_cylinder.x = ft_atoi_double(coors[0]);
	scene->cylinder->coor_cylinder.y = ft_atoi_double(coors[1]);
	scene->cylinder->coor_cylinder.z = ft_atoi_double(coors[2]);

	char **vects = ft_split(data[2], ',');
	scene->cylinder->vector_cylinder.x = ft_atoi_double(vects[0]);
	scene->cylinder->vector_cylinder.y = ft_atoi_double(vects[1]);
	scene->cylinder->vector_cylinder.z = ft_atoi_double(vects[2]);

	scene->cylinder->diameter_cylinder = ft_atoi_double(data[3]);
	
	scene->cylinder->height_cylinder = ft_atoi_double(data[4]);

	char **colors = ft_split(data[5], ',');
	scene->cylinder->color_cylinder->r = ft_atoi(colors[0]);
	scene->cylinder->color_cylinder->g = ft_atoi(colors[1]);
	scene->cylinder->color_cylinder->b = ft_atoi(colors[2]);
}


void	initialize_ambient_light(t_scene *scene)
{
    scene->ambient_light = malloc(sizeof(*(scene->ambient_light)));
    scene->ambient_light->color_ambient_light = malloc(sizeof(*(scene->ambient_light->color_ambient_light)));

}
void	initialize_camera(t_scene *scene)
{
    scene->camera = malloc(sizeof(*(scene->camera)));

}
void	initialize_light(t_scene *scene)
{
    scene->light = malloc(sizeof(*(scene->light)));

    scene->light->color_light = malloc(sizeof(*(scene->light->color_light)));

}
void	initialize_plane(t_scene *scene)
{
    scene->plane = malloc(sizeof(*(scene->plane)));

    scene->plane->color_plane = malloc(sizeof(*(scene->plane->color_plane)));

}
void	initialize_spher(t_scene *scene)
{
    scene->sphere = malloc(sizeof(*(scene->sphere)));

    scene->sphere->color_sphere = malloc(sizeof(*(scene->sphere->color_sphere)));

}
void	initialize_cylinder(t_scene *scene)
{
    scene->cylinder = malloc(sizeof(*(scene->cylinder)));

    scene->cylinder->color_cylinder = malloc(sizeof(*(scene->cylinder->color_cylinder)));

}