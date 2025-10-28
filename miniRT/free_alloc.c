#include "minirt.h"

void exit_error(char *str, char *scene_name, t_cleanup *cleanup)
{
    printf("Error\n%s in %s\n", str, scene_name);
    
    if (cleanup->tokens)
    {
        for (int i = 0; i < cleanup->token_count; i++)
        {
            if (cleanup->tokens[i])
                ft_free_split(cleanup->tokens[i]);
        }
        free(cleanup->tokens);
        cleanup->tokens = NULL;
    }
    
    if (cleanup->dispatched_table && *cleanup->dispatched_table)
    {
        ft_lstclear((void **)cleanup->dispatched_table, del, 'o');
        *cleanup->dispatched_table = NULL;
    }
    
    if (cleanup->scene)
    {
		free_scene(cleanup->scene);
        cleanup->scene = NULL;
    }
	if(cleanup->input_data)
	{
		free(cleanup->input_data);
		cleanup->input_data = NULL;	
	}
    if (cleanup->container)
    {
		// puts("container---------------------------------------------\n");
        ft_lstclear((void **)&cleanup->container, del, 'c');
        cleanup->container = NULL;
    }
    free(cleanup);
    
    exit(1);
}

void	del(void *content)
{
	free(content);
}

void	ft_free_split(char **str)
{
	int i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_scene(t_scene *scene)
{
	t_sphere *sp;
	t_sphere *next_sp;
	t_plane *pl;
	t_plane *next_pl;
	t_cylinder *cy;
	t_cylinder *next_cy;
	t_ambient_light *amb;
	t_ambient_light *next_amb;

	if (scene->ambient_light)
	{
		puts("amb freeid\n");
		free(scene->ambient_light->color_ambient_light);
		free(scene->ambient_light);
	}
	if (scene->camera)
	{
		puts("camera freeid\n");
		free(scene->camera->coor_camera);
		free(scene->camera->vector_camera);
		free(scene->camera);
	}
	if (scene->light)
	{
		puts("light freeid\n");
		free(scene->light->coor_light);
		free(scene->light->color_light);
		free(scene->light);
	}
	sp = scene->sphere;
	while (sp)
	{
		next_sp = sp->next;
		if (sp->flag_bump && sp->bump_texture)
		{
			puts("freeing sphere");
			if (sp->bump_texture->image)
			{
				if(sp->bump_texture->type == 2)
					mlx_delete_texture(sp->bump_texture->image);
				sp->bump_texture->image = NULL;
			}
			if (sp->img_path && sp->bump_texture->type == 2)
			{
				free(sp->img_path);
				sp->img_path = NULL;
			}
			free(sp->bump_texture);
			sp->bump_texture = NULL;
		}

		free(sp->color_sphere);
		free(sp->coor_sphere);
		free(sp);
		sp = next_sp;
	}
	pl = scene->plane;
	while (pl)
	{
		next_pl = pl->next;
		if (pl->flag_bump && pl->bump_texture)
		{
			puts("freeing plhere");
			if (pl->bump_texture->image && pl->bump_texture->type == 2)
			{
				mlx_delete_texture(pl->bump_texture->image);
				pl->bump_texture->image = NULL;
			}
			if (pl->img_path && pl->bump_texture->type == 2)
			{
				free(pl->img_path);
				pl->img_path = NULL;
			}
			free(pl->bump_texture);
			pl->bump_texture = NULL;
		}
		free(pl->color_plane);
		free(pl->coor_plane);
		free(pl->vector_plane);
		free(pl);
		pl = next_pl;
	}

	cy = scene->cylinder;
	while (cy)
	{
		next_cy = cy->next;
		// if(cy->flag_bump)
		// {
		// 	puts("freeeeeeeeeeeeeeeeeeeeeeeeeeing cyane\n");
		// 	if (cy->bump_texture && cy->bump_texture->image && cy->bump_texture->type == 2)
		// 	{
		// 		mlx_delete_texture(cy->bump_texture->image);
		// 		cy->bump_texture->image = NULL;
		// 	}
		// 	if (cy->img_path && cy->bump_texture->type == 2)
		// 		free(cy->img_path);
		// 	if (cy->bump_texture)
		// 		free(cy->bump_texture);

		// }
		free(cy->color_cylinder);
		free(cy->coor_cylinder);
		free(cy->vector_cylinder);
		free(cy);
		cy = next_cy;
	}
	free(scene);
}
