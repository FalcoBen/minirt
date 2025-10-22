#include "minirt.h"

// double  switch_ld(int color)
// {
//     return (color/255.0);
// }
// t_vec3  *switch_color_to_vec(t_color *color)
// {
//     t_vec3 *color_vec = malloc(sizeof(t_vec3));
//     color_vec->x = switch_ld(color->r);
//     color_vec->y = switch_ld(color->g);
//     color_vec->z = switch_ld(color->b);
//     return (color_vec);
// }
double sample_height(t_texture *tex, double u, double v)
{
    
    if (!tex) 
        return 0.0;

    if (tex->type == 1) 
    {
        int iu = (int)(u * 10) % 2;
        int iv = (int)(v * 10) % 2;
        return (iu == iv) ? 0.0 : 1.0;
    } 
    else if (tex->type == 2 && tex->image)
    {
        int x = (int)(u * tex->image->width) % tex->image->width;
        int y = (int)(v * tex->image->height) % tex->image->height;
        uint8_t *pixel = tex->image->pixels + (y * tex->image->width + x) * 4;  // RGBA
        return pixel[0] / 255.0;  // Red channel as height (grayscale)
    }
    return 0.0;
}

t_vec3 sample_color(t_texture *tex, double u, double v) 
{
    if (!tex)
        return (t_vec3){1.0, 1.0, 1.0};

    if (tex->type == 1) 
    { // Checkered fallback
        int iu = (int)(u * 10) % 2;
        int iv = (int)(v * 10) % 2;
        if (iu == iv)
        {
            return (t_vec3){0.0, 0.0, 0.0};
        }
        else
            return (t_vec3){1.0, 1.0, 1.0};
    } 
    else if (tex->type == 2 && tex->image)
    {
        int x = (int)(u * (double)tex->image->width) % tex->image->width;
        int y = (int)((1.0 - v) * (double)tex->image->height) % tex->image->height; // Flip V
        uint8_t *pixel = &tex->image->pixels[(y * tex->image->width + x) * 4]; // RGBA
        return (t_vec3){
            (double)pixel[0] / 255.0,
            (double)pixel[1] / 255.0,
            (double)pixel[2] / 255.0
        };
    }
    return (t_vec3){0.0, 0.0, 0.0};
}