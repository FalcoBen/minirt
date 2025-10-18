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