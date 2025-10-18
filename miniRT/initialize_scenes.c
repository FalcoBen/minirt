#include "minirt.h"

void	initialize_colors(t_color *colors)
{
	colors->r = 0;
	colors->g = 0;
	colors->b = 0;
}

void	initialize_coordinate(t_vec3 *coors)
{
	coors->x = 0;
	coors->y = 0;
	coors->z = 0;
	// coors->w = 1.0;
}

void	initialize_ambient_light(t_scene *scene)
{
    scene->ambient_light = malloc(sizeof(*(scene->ambient_light)));
    scene->ambient_light->color_ambient_light = malloc(sizeof(*(scene->ambient_light->color_ambient_light)));
	scene->ambient_light->bright_ambient_light = 0.0;
	initialize_colors(scene->ambient_light->color_ambient_light);
	scene->ambient_light->next = NULL;
}
void	initialize_camera(t_scene *scene)
{
    scene->camera = malloc(sizeof(*(scene->camera)));
	scene->camera->coor_camera = malloc(sizeof(*(scene->camera->coor_camera)));
	scene->camera->vector_camera = malloc(sizeof(*(scene->camera->vector_camera)));
	scene->camera->angle_view = 0;
	initialize_coordinate(scene->camera->coor_camera);
	initialize_coordinate(scene->camera->vector_camera);
	scene->camera->next = NULL;
}
void	initialize_light(t_scene *scene)
{
    scene->light = malloc(sizeof(*(scene->light)));
    scene->light->color_light = malloc(sizeof(*(scene->light->color_light)));
    scene->light->coor_light = malloc(sizeof(*(scene->light->coor_light)));

	initialize_colors(scene->light->color_light);
	initialize_coordinate(scene->light->coor_light);
	scene->light->bright_light = 0;
	scene->light->next = NULL;
}
void	initialize_plane(t_scene *scene)
{
    scene->plane = malloc(sizeof(*(scene->plane)));

    scene->plane->color_plane = malloc(sizeof(*(scene->plane->color_plane)));
    scene->plane->coor_plane = malloc(sizeof(*(scene->plane->coor_plane)));
    scene->plane->vector_plane = malloc(sizeof(*(scene->plane->vector_plane)));

	initialize_coordinate(scene->plane->vector_plane);
	initialize_coordinate(scene->plane->coor_plane);
	initialize_colors(scene->plane->color_plane);
	scene->plane->next = NULL;
}
void	initialize_spher(t_scene *scene)
{
    scene->sphere = malloc(sizeof(*(scene->sphere)));
    scene->sphere->color_sphere = malloc(sizeof(*(scene->sphere->color_sphere)));
    scene->sphere->coor_sphere = malloc(sizeof(*(scene->sphere->coor_sphere)));
	initialize_colors(scene->sphere->color_sphere);
	initialize_coordinate(scene->sphere->coor_sphere);
	scene->sphere->next = NULL;
}
void	initialize_cylinder(t_scene *scene)
{
    scene->cylinder = malloc(sizeof(*(scene->cylinder)));
    scene->cylinder->color_cylinder = malloc(sizeof(*(scene->cylinder->color_cylinder)));
    scene->cylinder->coor_cylinder = malloc(sizeof(*(scene->cylinder->coor_cylinder)));
    scene->cylinder->vector_cylinder = malloc(sizeof(*(scene->cylinder->vector_cylinder)));
	scene->cylinder->height_cylinder = 0;
	scene->cylinder->diameter_cylinder = 0;
	initialize_colors(scene->cylinder->color_cylinder);
	initialize_coordinate(scene->cylinder->coor_cylinder);
	initialize_coordinate(scene->cylinder->vector_cylinder);
	scene->cylinder->next = NULL;
}