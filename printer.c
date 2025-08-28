#include "minirt.h"


void	printer(t_scene *scene)
{
	t_scene *curr = scene;
	int i = 0;
	while(i < 1)
	{
		if(curr->ambient_light)
		{
			printf("A : AMBIENT LIGHT\n");
			printf("bright_ambient light =  %f\n", curr->ambient_light->bright_ambient_light);
			t_color *tmp = curr->ambient_light->color_ambient_light;
			printf("R = %d\n", tmp->r);
			printf("G = %d\n", tmp->g);
			printf("B = %d\n", tmp->b);
			printf("\n\n");
		}
		if(curr->camera)
		{
			printf("C : CAMERA\n");
			printf("--------COORDONNATE--------\n");
			t_coordonnate *tmp = curr->camera->coor_camera;
			printf("X = %f\n", tmp->x);
			printf("Y = %f\n", tmp->y);
			printf("Z = %f\n", tmp->z);
			
			printf("**********VECTORS**********\n");
			t_coordonnate *tmp_vec = curr->camera->vector_camera;
			printf("X = %f\n", tmp_vec->x);
			printf("Y = %f\n", tmp_vec->y);
			printf("Z = %f\n", tmp_vec->z);
			printf("camera angele_view =  %f\n", curr->camera->angle_view);
			printf("\n\n");

		}
		if(curr->light)
		{
			printf("L : LIGHT \n");
			printf("--------COORDONNATE--------\n");
			t_coordonnate *tmp = curr->light->coor_light;
			printf("X = %f\n", tmp->x);
			printf("Y = %f\n", tmp->y);
			printf("Z = %f\n", tmp->z);
			
			printf("bright_light = %f\n", curr->light->bright_light);

			printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
			t_color *tmp_color = curr->light->color_light;
			printf("R = %d\n", tmp_color->r);
			printf("G = %d\n", tmp_color->g);
			printf("B = %d\n", tmp_color->b);
			printf("\n\n");
		}
		if(curr->plane)
		{
			printf("pl : PLANE \n");
			printf("--------COORDONNATE--------\n");
			t_coordonnate *tmp = curr->plane->coor_plane;
			printf("X = %f\n", tmp->x);
			printf("Y = %f\n", tmp->y);
			printf("Z = %f\n", tmp->z);
			
			printf("**********VECTORS**********\n");
			t_coordonnate *tmp_vec = curr->plane->vector_plane;
			printf("X = %f\n", tmp_vec->x);
			printf("Y = %f\n", tmp_vec->y);
			printf("Z = %f\n", tmp_vec->z);

			printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
			t_color *tmp_color = curr->plane->color_plane;
			printf("R = %d\n", tmp_color->r);
			printf("G = %d\n", tmp_color->g);
			printf("B = %d\n", tmp_color->b);
			printf("\n\n");
		}
		if(curr->sphere)
		{
			printf("sp : SPHERE\n");
			printf("--------COORDONNATE--------\n");
			t_coordonnate *tmp = curr->sphere->coor_sphere;
			printf("X = %f\n", tmp->x);
			printf("Y = %f\n", tmp->y);
			printf("Z = %f\n", tmp->z);
			
			printf("dametere_sphere =  %f\n", curr->sphere->diameter_sphere);

			printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
			t_color *tmp_color = curr->sphere->color_sphere;
			printf("R = %d\n", tmp_color->r);
			printf("G = %d\n", tmp_color->g);
			printf("B = %d\n", tmp_color->b);
			printf("\n\n");
		}
		if(curr->cylinder)
		{
			printf("cy : CYLINDER \n");
			printf("--------COORDONNATE--------\n");
			t_coordonnate *tmp = curr->cylinder->coor_cylinder;
			printf("X = %f\n", tmp->x);
			printf("Y = %f\n", tmp->y);
			printf("Z = %f\n", tmp->z);
			
			printf("**********VECTORS**********\n");
			t_coordonnate *tmp_vec = curr->cylinder->vector_cylinder;
			printf("x = %f\n", tmp_vec->x);
			printf("y = %f\n", tmp_vec->y);
			printf("z = %f\n", tmp_vec->z);

			printf("diameter_cylinder = %f\n", curr->cylinder->diameter_cylinder);
			printf("height_cylinder = %f\n", curr->cylinder->height_cylinder);

			printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
			t_color *tmp_color = curr->cylinder->color_cylinder;
			printf("R = %d\n", tmp_color->r);
			printf("G = %d\n", tmp_color->g);
			printf("B = %d\n", tmp_color->b);
			printf("\n*******END******\n");
			printf("\n\n");
		}
		printf("\n==========================================================\n");
		i++;
	}
}