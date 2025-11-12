#include "parsing.h"

#include "parsing.h"

void printer(t_scene *scene)
{
    if(scene->ambient_light)
    {
		t_ambient_light_fb *am = scene->ambient_light;
        while(am)
		{

			printf("A : AMBIENT LIGHT\n");
			printf("bright_ambient light =  %f\n", am->bright_ambient_light_fb);
			t_color_fb *tmp = am->color_ambient_light;
			printf("R = %d\n", tmp->r); 
			printf("G = %d\n", tmp->g);
			printf("B = %d\n", tmp->b);
			printf("\n\n");
			am = am->next;
		}
    }
    
    if(scene->camera)
    {
        t_camera_fb *camera = scene->camera;   
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
        t_light_fb *light = scene->light;
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
            t_color_fb *tmp_color = light->color_light;
            printf("R = %d\n", tmp_color->r);
            printf("G = %d\n", tmp_color->g);
            printf("B = %d\n", tmp_color->b);
            printf("\n\n");
            light = light->next;
        }
    }
    
    if(scene->plane)
    {
        t_plane_fb *plane = scene->plane;
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
            t_color_fb *tmp_color = plane->color_plane;
            printf("R = %d\n", tmp_color->r);
            printf("G = %d\n", tmp_color->g);
            printf("B = %d\n", tmp_color->b);
            printf("\n\n");
            plane = plane->next;
        }
    }
    
    if(scene->sphere)
    {
        t_sphere_fb *sphere = scene->sphere;
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
            t_color_fb *tmp_color = sphere->color_sphere;
            printf("R = %d\n", tmp_color->r);
            printf("G = %d\n", tmp_color->g);
            printf("B = %d\n", tmp_color->b);
            printf("\n\n");
            sphere = sphere->next;
        }
    }
    
    if(scene->cylinder)
    {
        t_cylinder_fb *cylinder = scene->cylinder;
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
            printf("height_cylinder_fb = %f\n", cylinder->height_cylinder_fb);
            printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
            t_color_fb *tmp_color = cylinder->color_cylinder;
            printf("R = %d\n", tmp_color->r);
            printf("G = %d\n", tmp_color->g);
            printf("B = %d\n", tmp_color->b);
            printf("\n\n");
            printf("%d\n", cylinder->closed);
            cylinder = cylinder->next;
        }
        printf("\n*******END******\n");
    }
    if(scene->cone)
    {
        t_cone_fb *cone = scene->cone;
        while(cone)
        {
            printf("cone : cone \n");
            printf("--------COORDONNATE--------\n");
            t_vec3 *tmp = cone->coor_cone;
            printf("X = %f\n", tmp->x);
            printf("Y = %f\n", tmp->y);
            printf("Z = %f\n", tmp->z);
            
            printf("**********VECTORS**********\n");
            t_vec3 *tmp_vec = cone->vector_cone;
            printf("x = %f\n", tmp_vec->x);
            printf("y = %f\n", tmp_vec->y);
            printf("z = %f\n", tmp_vec->z);
            printf("diameter_cone = %f\n", cone->minimum);
            printf("height_cone_fb = %f\n", cone->maximum);
            printf("$$$$$$$$$$$COLORS$$$$$$$$$$$\n");
            t_color_fb *tmp_color = cone->color_cone;
            printf("R = %d\n", tmp_color->r);
            printf("G = %d\n", tmp_color->g);
            printf("B = %d\n", tmp_color->b);
            printf("\n\n");
            printf("min   %f\n", cone->minimum);
            printf("max   %f\n", cone->maximum);
            printf("angle %f\n", cone->angle);
            printf("%d\n", cone->closed_flag);

            cone = cone->next;
        }
        printf("\n*******END******\n");
    }
    
    printf("\n==========================================================\n");
}