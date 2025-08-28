#include "minirt.h"

#include "minirt.h"

void printer(t_scene *scene)
{
    if(scene->ambient_light)
    {
		t_ambient_light *am = scene->ambient_light;
        while(am)
		{

			printf("A : AMBIENT LIGHT\n");
			printf("bright_ambient light =  %f\n", am->bright_ambient_light);
			t_color *tmp = am->color_ambient_light;
			printf("R = %d\n", tmp->r);
			printf("G = %d\n", tmp->g);
			printf("B = %d\n", tmp->b);
			printf("\n\n");
			am = am->next;
		}
    }
    
    if(scene->camera)
    {
        printf("C : CAMERA\n");
        printf("--------COORDONNATE--------\n");
        t_coordonnate *tmp = scene->camera->coor_camera;
        printf("X = %f\n", tmp->x);
        printf("Y = %f\n", tmp->y);
        printf("Z = %f\n", tmp->z);
        
        printf("**********VECTORS**********\n");
        t_coordonnate *tmp_vec = scene->camera->vector_camera;
        printf("X = %f\n", tmp_vec->x);
        printf("Y = %f\n", tmp_vec->y);
        printf("Z = %f\n", tmp_vec->z);
        printf("camera angele_view =  %f\n", scene->camera->angle_view);
        printf("\n\n");
    }
    
    if(scene->light)
    {
        printf("L : LIGHT \n");
        printf("--------COORDONNATE--------\n");
        t_coordonnate *tmp = scene->light->coor_light;
        printf("X = %f\n", tmp->x);
        printf("Y = %f\n", tmp->y);
        printf("Z = %f\n", tmp->z);
        
        printf("bright_light = %f\n", scene->light->bright_light);
        printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
        t_color *tmp_color = scene->light->color_light;
        printf("R = %d\n", tmp_color->r);
        printf("G = %d\n", tmp_color->g);
        printf("B = %d\n", tmp_color->b);
        printf("\n\n");
    }
    
    if(scene->plane)
    {
        printf("pl : PLANE \n");
        printf("--------COORDONNATE--------\n");
        t_coordonnate *tmp = scene->plane->coor_plane;
        printf("X = %f\n", tmp->x);
        printf("Y = %f\n", tmp->y);
        printf("Z = %f\n", tmp->z);
        
        printf("**********VECTORS**********\n");
        t_coordonnate *tmp_vec = scene->plane->vector_plane;
        printf("X = %f\n", tmp_vec->x);
        printf("Y = %f\n", tmp_vec->y);
        printf("Z = %f\n", tmp_vec->z);
        printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
        t_color *tmp_color = scene->plane->color_plane;
        printf("R = %d\n", tmp_color->r);
        printf("G = %d\n", tmp_color->g);
        printf("B = %d\n", tmp_color->b);
        printf("\n\n");
    }
    
    if(scene->sphere)
    {
        printf("sp : SPHERE\n");
        printf("--------COORDONNATE--------\n");
        t_coordonnate *tmp = scene->sphere->coor_sphere;
        printf("X = %f\n", tmp->x);
        printf("Y = %f\n", tmp->y);
        printf("Z = %f\n", tmp->z);
        
        printf("dametere_sphere =  %f\n", scene->sphere->diameter_sphere);
        printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
        t_color *tmp_color = scene->sphere->color_sphere;
        printf("R = %d\n", tmp_color->r);
        printf("G = %d\n", tmp_color->g);
        printf("B = %d\n", tmp_color->b);
        printf("\n\n");
    }
    
    if(scene->cylinder)
    {
        printf("cy : CYLINDER \n");
        printf("--------COORDONNATE--------\n");
        t_coordonnate *tmp = scene->cylinder->coor_cylinder;
        printf("X = %f\n", tmp->x);
        printf("Y = %f\n", tmp->y);
        printf("Z = %f\n", tmp->z);
        
        printf("**********VECTORS**********\n");
        t_coordonnate *tmp_vec = scene->cylinder->vector_cylinder;
        printf("x = %f\n", tmp_vec->x);
        printf("y = %f\n", tmp_vec->y);
        printf("z = %f\n", tmp_vec->z);
        printf("diameter_cylinder = %f\n", scene->cylinder->diameter_cylinder);
        printf("height_cylinder = %f\n", scene->cylinder->height_cylinder);
        printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
        t_color *tmp_color = scene->cylinder->color_cylinder;
        printf("R = %d\n", tmp_color->r);
        printf("G = %d\n", tmp_color->g);
        printf("B = %d\n", tmp_color->b);
        printf("\n*******END******\n");
        printf("\n\n");
    }
    
    printf("\n==========================================================\n");
}