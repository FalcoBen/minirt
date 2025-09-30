#include "minirt.h"
#include "camera.h" // Include the camera header
#include <math.h>

// Add this debug function
void print_camera_debug(t_virtual_camera *cam)
{
    printf("=== CAMERA DEBUG ===\n");
    printf("Origin: (%f, %f, %f)\n", cam->origin.x, cam->origin.y, cam->origin.z);
    printf("Forward: (%f, %f, %f)\n", cam->forward.x, cam->forward.y, cam->forward.z);
    printf("Right: (%f, %f, %f)\n", cam->right.x, cam->right.y, cam->right.z);
    printf("Up: (%f, %f, %f)\n", cam->up.x, cam->up.y, cam->up.z);
    printf("Viewport: %f x %f\n", cam->viewport_width, cam->viewport_height);
    printf("Lower Left: (%f, %f, %f)\n", cam->lower_left.x, cam->lower_left.y, cam->lower_left.z);
    printf("====================\n");
}

void print_ray_debug(t_ray *ray, int x, int y)
{
    if (x == 400 && y == 400) // Print for center pixel only
    {
        printf("=== RAY DEBUG (pixel 400,400) ===\n");
        printf("Origin: (%f, %f, %f)\n", ray->origin.x, ray->origin.y, ray->origin.z);
        printf("Direction: (%f, %f, %f)\n", ray->direction.x, ray->direction.y, ray->direction.z);
        printf("=================================\n");
    }
}

void print_plane_debug(t_plane *plane)
{
    printf("=== PLANE DEBUG ===\n");
    printf("Point: (%f, %f, %f)\n", plane->coor_plane->x, plane->coor_plane->y, plane->coor_plane->z);
    printf("Normal: (%f, %f, %f)\n", plane->vector_plane->x, plane->vector_plane->y, plane->vector_plane->z);
    printf("Color: (%d, %d, %d)\n", plane->color_plane->r, plane->color_plane->g, plane->color_plane->b);
    printf("===================\n");
}
