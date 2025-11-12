#include "../../MiniRt.h"



t_stack_ray s_ray_for_camera(t_camera *cam, ld x, ld y)
{
    t_stack_ray ray;
    
    // The offset from the edge of the canvas to the pixel's center
    ld xoffset = (x + 0.5) * cam->pixel_size;
    ld yoffset = (y + 0.5) * cam->pixel_size;
    
    // The untransformed coordinates of the pixel in world space
    // (camera looks toward -z, so +x is to the left)
    ld world_x = cam->half_width - xoffset;
    ld world_y = cam->half_height - yoffset;
    
    // Using the camera matrix, transform the canvas point and the origin,
    // and then compute the ray's direction vector
    // (remember that the canvas is at z = -1)
    t_tuple t = s_create_tuple((ld[3]){world_x, world_y, -1}, 1);
    t_matrix tmp = s_matrix_tuple(cam->tran_inv, &t);
    t_tuple pixel = s_matrix_to_tuple(&tmp);
    
    t_tuple t1 = s_create_tuple((ld[3]){0, 0, 0}, 1);
    t_matrix tmp1 = s_matrix_tuple(cam->tran_inv, &t1);
    t_tuple origin = s_matrix_to_tuple(&tmp1);
    
    t_tuple direction;
    s_sub_t(&direction, pixel, origin);
    s_vec_norm(&direction);
    
    ray = s_ray_constrctor(&origin, &direction);
    return ray;
}





/*
* hsize :  the horizontal size (in pixels) of the canvas that the picture will be
*   rendered to.
* vsize : is the canvas’s vertical size (in pixels).
* field_ov : angle that describes how much the camera can see
*/
t_camera *camera(ld hsize, ld vsize, ld field_ov, t_matrix *m)
{
    t_camera *new;

    ld half_view = tanf(field_ov/2);
    new = alloc(sizeof(t_camera), false);
    new->hsize = hsize;
    new->vsize = vsize;
    new->field_ov = field_ov;
    
    if(!m)
        new->tran = identity_matrix(4, 4);
    else
        new->tran = m;
    new->tran_inv = matrix_inverte(new->tran);

    new->aspect = hsize / vsize;
    
    // Calculate half_width and half_height based on aspect ratio
    if (new->aspect >= 1)
    {
        // Landscape or square: width >= height
        new->half_width = half_view;
        new->half_height = half_view / new->aspect;
    }
    else
    {
        // Portrait: height > width
        new->half_width = new->aspect * half_view;
        new->half_height = half_view;
    }
    
    // ✅ FIXED: Pixel size should be based on half_width, not half_view!
    // This is because half_width already accounts for aspect ratio
    new->pixel_size = (new->half_width * 2) / hsize;
    
    printf("++++++++++> pixel_size: %f\n", new->pixel_size);
    printf("++++++++++> half_width: %f\n", new->half_width);
    printf("++++++++++> half_height: %f\n", new->half_height);
    printf("++++++++++> aspect: %f\n", new->aspect);
    
    printf("new->tran or view matrix\n");
    for(int i = 0; i < new->tran->row; i++)
    {
        for(int j = 0; j < new->tran->col; j++)
            printf("%f , ", new->tran->stack_matrix[i][j]);
        printf("\n");
    }
    printf("new->tran_inv\n");
    for(int i = 0; i < new->tran_inv->row; i++)
    {
        for(int j = 0; j < new->tran_inv->col; j++)
            printf("%f , ", new->tran_inv->stack_matrix[i][j]);
        printf("\n");
    }
    printf("===================\n");
    
    copy_matrix_contant(new->tran);
    copy_matrix_contant(new->tran_inv);
    return new;
}


