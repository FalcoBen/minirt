#include "../../MiniRt.h"
#include <limits.h>
t_material *material()
{
    t_material *m;

    m = alloc(sizeof(t_material), false);
    m->color = alloc(sizeof(t_color), false);
    // m->ambient = 0.1;
    // m->diffuse = 0.9;
    // m->specular = 0.9;
    // m->shininess = 200;
    return m;
}

t_light *point_light(t_color *color, t_tuple *position)
{
    t_light *light;
    if (!color || !position)
        return NULL;
    light = alloc(sizeof(t_light), false);
    light->intensity = color;
    light->position = position;
    return light;
}

// object_point = s->inv * point   (world → object space)
// object_normal = object_point - (0,0,0,1)

// world_normal = transpose(s->inv) * object_normal
// world_normal.w = 0   (make sure it stays a vector)
t_tuple *normal_at(t_sphere *s, t_tuple *point)
{
    if (!s || !point)
        return NULL;
    t_tuple *obj_point = matrix_to_tuple(matrix_tuple(s->inv, point));
    t_tuple *obj_normal = sub_t(obj_point, s->origin);
    obj_normal->w = 0;
    t_tuple *world_normal = matrix_to_tuple(matrix_tuple(matrix_transpos(s->inv), obj_normal));
    world_normal->w = 0;
    return vec_norm(world_normal);
}

// return in - normal * 2 * dot(in, normal)
t_tuple *reflect(t_tuple *in, t_tuple *normal)
{
    t_tuple *tmp;
    if (!in || !normal)
        return NULL;
    ld t = (2 * dot_product(in, normal));
    tmp = scalar_mult(normal, t);
    return (sub_t(in, tmp));
}

t_color *create_color(ld *vals)
{
    t_color *color;

    color = alloc(sizeof(t_color), false);
    color->r = vals[0];
    color->g = vals[1];
    color->b = vals[2];
    return color;
}

t_color *hadamard(t_color *c1, t_color *c2)
{
    t_color *res = alloc(sizeof(t_color), false);
    res->r = c1->r * c2->r;
    res->g = c1->g * c2->g;
    res->b = c1->b * c2->b;
    return res;
}

t_color *color_scalar(t_color *c, ld s)
{
    t_color *res = alloc(sizeof(t_color), false);
    res->r = c->r * s;
    res->g = c->g * s;
    res->b = c->b * s;
    return res;
}

t_color *color_add(t_color *c1, t_color *c2)
{
    t_color *res = alloc(sizeof(t_color), false);
    res->r = c1->r + c2->r;
    res->g = c1->g + c2->g;
    res->b = c1->b + c2->b;
    return res;
}

// int32_t combine_color(t_color c)
// {
//     int r = (int)fmin(fmax(c.r * 255, 0), 255);
//     int g = (int)fmin(fmax(c.g * 255, 0), 255);
//     int b = (int)fmin(fmax(c.b * 255, 0), 255);
//     return (r << 24 | g << 16 | b << 8 | 255);
// }

// t_color *split_color(int c)
// {
//     t_color *color = alloc(sizeof(t_color), false);

//     color->r = (c >> 24) & 0xFF;
//     color->g = (c >> 16) & 0xFF;
//     color->b = (c >> 8)  & 0xFF;
//     return color;
// }
// int open_file()
// {
//     int fd = open("color.txt", O_CREAT | 0777);
//     if(fd < 0)
//         exit(1);
//     return fd;
// }
static inline ld tone_map(ld x)
{
    return (x / (1.0 + x));
}

static inline ld gamma_correct(ld x, ld gamma)
{
    return pow(x, 1.0 / gamma);
}

t_color *lighting(t_material *material, t_light *light, 
    t_tuple *position, t_tuple *eyev, t_tuple *normalv)
{

    t_color ambient  = (t_color){0, 0, 0};
    t_color diffuse  = (t_color){0, 0, 0};
    t_color specular = (t_color){0, 0, 0};
    // printf("m: %p, l: %p, p: %p, e: %p, n: %p\n", material, light, position, eyev, normalv);
    // Effective color = surface × light

    t_color effective_color = *hadamard(material->color, light->intensity);
    // dprintf(fd ,"material_color : {%Lf, %Lf, %Lf}\n", material->color->r, material->color->g, material->color->b);
    // Light vector
    t_tuple *lightv = vec_norm(sub_t(position, light->position));
    // dprintf(fd ,"lightv : {%Lf, %Lf, %Lf}\n", lightv->x, lightv->y, lightv->z);
    
    ambient = *color_scalar(&effective_color, material->ambient);
    // dprintf(fd ,"ambient_color : {%Lf, %Lf, %Lf}\n", ambient.r, ambient.g, ambient.b);

    
    ld light_dot_normal = dot_product(lightv, normalv);
    if (light_dot_normal >= 0)
    {
        diffuse = *color_scalar(&effective_color,
            material->diffuse * light_dot_normal);
        // dprintf(fd ,"diffuse_color : {%Lf, %Lf, %Lf}\n", diffuse.r, diffuse.g, diffuse.b);
        t_tuple *reflectv = reflect(neg_t(lightv), normalv);
        ld reflect_dot_eye = dot_product(reflectv, eyev);
        if (reflect_dot_eye > 0)
        {
            ld factor = pow(reflect_dot_eye, material->shininess);
            specular = *color_scalar(light->intensity,
                                    (material->specular * factor));
            // dprintf(fd ,"specular_color : {%Lf, %Lf, %Lf}\n", specular.r, specular.g, specular.b);
            
            // dprintf(fd_reflection,
            //     "reflect_dot_eye = %Lf ||| specular_r = %Lf || specular_g = %Lf || specular_b = %Lf \n",
            //     reflect_dot_eye, specular.r, specular.g, specular.b);
        }
    }
    t_color *result = color_add(color_add(&ambient, &diffuse), &specular);
    // printf("result_r = %Lf || result_g = %Lf || result_b = %Lf \n",
    //         result->r, result->g, result->b);
    // if (light_dot_normal > 0)
    result->r = gamma_correct(tone_map(result->r), 1);
    result->g = gamma_correct(tone_map(result->g), 1);
    result->b = gamma_correct(tone_map(result->b), 1);

    // dprintf(fd, "********************************************\n");
    return result;
}



// int main()
// {
//     t_sphere *s;
//     t_tuple *position;
//     t_tuple *eyev;
//     t_tuple *normalv;
//     t_material *m;
//     t_color color = {1, 1, 1};

//     ld v_coor[3] = {0, 0, -1};
//     eyev = create_vector(v_coor);
//     ld v_coor_1[3] = {0, 0, -1};
//     normalv = create_vector(v_coor_1);

//     ld p_coor_1[3] = {0, 0, 0};
//     position = create_point(p_coor_1);

//     ld m_coor[4] = {0.1, 0.9, 0.9, 200};
//     m = material(m_coor, &color);

//     ld p_coor[3] = {0, 10, -10};
//     t_light *light = point_light(&color, create_point(p_coor));
//     t_color *result = lighting(m, light, position, eyev, normalv);
//     // set_transform(&s, translation(0, 1, 0));
//     // t_matrix *m = translation(0, 1, 0);
//     // t_matrix *m =  matrix_multi(scaling(1, 0.5, 1), rotation_z(M_PI/5));
//     // sphere(&s, m, true);
//     // t_tuple *normal = reflect(p, p1);
//     printf("%Lf || %Lf || %Lf \n", result->r, result->g, result->b);
// }