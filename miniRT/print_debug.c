#include "minirt.h"
#include "camera.h"

void debug_rendering(t_scene *scene, t_virtual_camera *cam, mlx_image_t *img, int x, int y, t_ray ray, double closest_t, t_hit_record hit_rec, t_color final_color, int hit)
{
    printf("=== Debug at Pixel (%d, %d) ===\n", x, y);

    // Camera and Ray Debug
    printf("Camera Origin: (%f, %f, %f)\n", cam->origin.x, cam->origin.y, cam->origin.z);
    printf("Forward: (%f, %f, %f)\n", cam->forward.x, cam->forward.y, cam->forward.z);
    printf("Right: (%f, %f, %f)\n", cam->right.x, cam->right.y, cam->right.z);
    printf("Up: (%f, %f, %f)\n", cam->up.x, cam->up.y, cam->up.z);
    printf("Viewport Width: %f, Height: %f\n", cam->viewport_width, cam->viewport_height);
    printf("Ray Origin: (%f, %f, %f), Direction: (%f, %f, %f)\n", ray.origin.x, ray.origin.y, ray.origin.z, ray.direction.x, ray.direction.y, ray.direction.z);

    // Intersection Debug
    printf("Closest t: %f, Hit: %d\n", closest_t, hit);
    if (hit)
    {
        printf("Hit Point: (%f, %f, %f)\n", hit_rec.p.x, hit_rec.p.y, hit_rec.p.z);
        printf("Normal: (%f, %f, %f)\n", hit_rec.normal.x, hit_rec.normal.y, hit_rec.normal.z);
        printf("Color from Hit: (%d, %d, %d)\n", hit_rec.color.r, hit_rec.color.g, hit_rec.color.b);
    }

    // Scene Object Debug
    printf("Sphere at: (%f, %f, %f), Radius: %f\n", scene->sphere->coor_sphere->x, scene->sphere->coor_sphere->y, scene->sphere->coor_sphere->z, scene->sphere->diameter_sphere / 2.0);
    t_plane *p = scene->plane;
    int plane_count = 0;
    while (p)
    {
        printf("Plane %d at: (%f, %f, %f), Normal: (%f, %f, %f)\n", plane_count, p->coor_plane->x, p->coor_plane->y, p->coor_plane->z, p->vector_plane->x, p->vector_plane->y, p->vector_plane->z);
        p = p->next;
        plane_count++;
    }

    // Final Color Debug
    printf("Final Color: (%d, %d, %d)\n", final_color.r, final_color.g, final_color.b);
    printf("---------------------------\n");
}

