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
        t_camera *camera = scene->camera;   
        printf("C : CAMERA\n");
        printf("--------COORDONNATE--------\n");
        t_vec3 *tmp = camera->coor_camera;
        printf("X = %f\n", tmp->x);
        printf("Y = %f\n", tmp->y);
        printf("Z = %f\n", tmp->z);
        
        printf("**********VECTORS**********\n");
        t_vec3 *tmp_vec = camera->vector_camera;
        printf("X = %f\n", tmp_vec->x);
        printf("Y = %f\n", tmp_vec->y);
        printf("Z = %f\n", tmp_vec->z);
        printf("camera angele_view =  %f\n", camera->angle_view);
        printf("\n\n");
    }
    
    if(scene->light)
    {
        t_light *light = scene->light;
        while(light)
        {
            printf("L : LIGHT \n");
            printf("--------COORDONNATE--------\n");
            t_vec3 *tmp = light->coor_light;
            printf("X = %f\n", tmp->x);
            printf("Y = %f\n", tmp->y);
            printf("Z = %f\n", tmp->z);
            
            printf("bright_light = %f\n", light->bright_light);
            printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
            t_color *tmp_color = light->color_light;
            printf("R = %d\n", tmp_color->r);
            printf("G = %d\n", tmp_color->g);
            printf("B = %d\n", tmp_color->b);
            printf("\n\n");
            light = light->next;
        }
    }
    
    if(scene->plane)
    {
        t_plane *plane = scene->plane;
        while(plane)
        {
            printf("pl : PLANE \n");
            printf("--------COORDONNATE--------\n");
            t_vec3 *tmp = plane->coor_plane;
            printf("X = %f\n", tmp->x);
            printf("Y = %f\n", tmp->y);
            printf("Z = %f\n", tmp->z);
            
            printf("**********VECTORS**********\n");
            t_vec3 *tmp_vec = plane->vector_plane;
            printf("X = %f\n", tmp_vec->x);
            printf("Y = %f\n", tmp_vec->y);
            printf("Z = %f\n", tmp_vec->z);
            printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
            t_color *tmp_color = plane->color_plane;
            printf("R = %d\n", tmp_color->r);
            printf("G = %d\n", tmp_color->g);
            printf("B = %d\n", tmp_color->b);
            printf("\n\n");
            plane = plane->next;
        }
    }
    
    if(scene->sphere)
    {
        t_sphere *sphere = scene->sphere;
        while(sphere)
        {
            printf("sp : SPHERE\n");
            printf("--------COORDONNATE--------\n");
            t_vec3 *tmp = sphere->coor_sphere;
            printf("X = %f\n", tmp->x);
            printf("Y = %f\n", tmp->y);
            printf("Z = %f\n", tmp->z);
            
            printf("dametere_sphere =  %f\n", sphere->diameter_sphere);
            printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
            t_color *tmp_color = sphere->color_sphere;
            printf("R = %d\n", tmp_color->r);
            printf("G = %d\n", tmp_color->g);
            printf("B = %d\n", tmp_color->b);
            printf("\n\n");
            sphere = sphere->next;
        }
    }
    
    if(scene->cylinder)
    {
        t_cylinder *cylinder = scene->cylinder;
        while(cylinder)
        {
            printf("cy : CYLINDER \n");
            printf("--------COORDONNATE--------\n");
            t_vec3 *tmp = cylinder->coor_cylinder;
            printf("X = %f\n", tmp->x);
            printf("Y = %f\n", tmp->y);
            printf("Z = %f\n", tmp->z);
            
            printf("**********VECTORS**********\n");
            t_vec3 *tmp_vec = cylinder->vector_cylinder;
            printf("x = %f\n", tmp_vec->x);
            printf("y = %f\n", tmp_vec->y);
            printf("z = %f\n", tmp_vec->z);
            printf("diameter_cylinder = %f\n", cylinder->diameter_cylinder);
            printf("height_cylinder = %f\n", cylinder->height_cylinder);
            printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
            t_color *tmp_color = cylinder->color_cylinder;
            printf("R = %d\n", tmp_color->r);
            printf("G = %d\n", tmp_color->g);
            printf("B = %d\n", tmp_color->b);
            printf("\n\n");
            cylinder = cylinder->next;
        }
        printf("\n*******END******\n");
    }
    
    printf("\n==========================================================\n");
}