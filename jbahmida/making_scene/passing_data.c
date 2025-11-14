#include "../MiniRt.h"
#include "../../fbenalla/miniRT/parsing.h"





// void validate_scene(t_scene *scene)
// {
// 	printf("=== Validating Scene ===\n");
	
// 	if (!scene->camera)
// 	{
// 		fprintf(stderr, "No camera in scene!\n");
// 		exit(1);
// 	}
// 	printf("Camera: pos(%.2f,%.2f,%.2f) orient(%.2f,%.2f,%.2f) FOV=%.2f\n",
// 		scene->camera->coor_camera->x,
// 		scene->camera->coor_camera->y,
// 		scene->camera->coor_camera->z,
// 		scene->camera->vector_camera->x,
// 		scene->camera->vector_camera->y,
// 		scene->camera->vector_camera->z,
// 		scene->camera->angle_view);
	
// 	if (!scene->ambient_light)
// 	{
// 		fprintf(stderr, "No ambient light in scene!\n");
// 		exit(1);
// 	}
// 	printf("Ambient light: brightness=%.2f color(%d,%d,%d)\n",
// 		scene->ambient_light->bright_ambient_light_fb,
// 		scene->ambient_light->color_ambient_light->r,
// 		scene->ambient_light->color_ambient_light->g,
// 		scene->ambient_light->color_ambient_light->b);
	
// 	int sphere_count = 0;
// 	t_sphere_fb *sp = scene->sphere;
// 	while (sp)
// 	{
// 		// validate_sphere_data(sp);
// 		sphere_count++;
// 		sp = sp->next;
// 	}
// 	printf("Total spheres: %d\n", sphere_count);
	
// 	int plane_count = 0;
// 	t_plane_fb *pl = scene->plane;
// 	while (pl)
// 	{
// 		printf("Plane: pos(%.2f,%.2f,%.2f) normal(%.2f,%.2f,%.2f) color(%d,%d,%d)\n",
// 			pl->coor_plane->x, pl->coor_plane->y, pl->coor_plane->z,
// 			pl->vector_plane->x, pl->vector_plane->y, pl->vector_plane->z,
// 			pl->color_plane->r, pl->color_plane->g, pl->color_plane->b);
// 		plane_count++;
// 		pl = pl->next;
// 	}
// 	printf("Total planes: %d\n", plane_count);
	
// }
void	loop_lights(t_world *world, t_light_fb *current_light, int *i)
{
	world->lights[*i] = malloc(sizeof(t_light));
	world->lights[*i]->intensity = malloc(sizeof(t_color));
	world->lights[*i]->intensity->r = current_light->color_light->r / 255.0;
	world->lights[*i]->intensity->g = current_light->color_light->g / 255.0;
	world->lights[*i]->intensity->b = current_light->color_light->b / 255.0;
	world->lights[*i]->intensity->r *= current_light->bright_light;
	world->lights[*i]->intensity->g *= current_light->bright_light;
	world->lights[*i]->intensity->b *= current_light->bright_light;
	world->lights[*i]->position = malloc(sizeof(t_tuple));
	world->lights[*i]->position->x = current_light->coor_light->x;
	world->lights[*i]->position->y = current_light->coor_light->y;
	world->lights[*i]->position->z = current_light->coor_light->z;
	world->lights[*i]->position->w = 1;
}

void	light_obj_creator(t_world *world, t_scene *scene)
{
	int			i;
	t_light_fb	*light_count;
	t_light_fb	*current_light;

	light_count = scene->light;
	i = 0;
	world->light_num = 0;
	while (light_count)
	{
		world->light_num++;
		light_count = light_count->next;
	}
	if (world->light_num > 0)
	{
		world->lights = malloc(sizeof(t_light *) * world->light_num);
		if (!world->lights)
		{
			fprintf(stderr, "Failed to allocate world->lights\n");
			exit(1);
		}
		current_light = scene->light;
		while (current_light && i < world->light_num)
		{
			loop_lights(world, current_light, &i);
			current_light = current_light->next;
			i++;
		}
	}
}





t_world	s_world(t_scene *scene)
{
	t_world	world;
	int		i;

	light_obj_creator(&world, scene);
	world.obj_num = number_of_all_objects(scene);
	world.objects = malloc(sizeof(t_object *) * (world.obj_num + 1));
	if (!world.objects)
	{
		fprintf(stderr, "Failed to allocate world.objects\n");
		exit(1);
	}
	i = 0;
	s_world_sphere_constractor(&world, scene, &i);
	s_world_plane_constractor(&world, scene, &i);
	s_world_cylinder_constractor(&world, scene, &i);
	s_world_cone_constractor(&world, scene, &i);
	world.objects[i] = NULL;
	return (world);
}
