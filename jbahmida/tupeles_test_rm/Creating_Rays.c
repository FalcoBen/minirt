#include "../../MiniRt.h"

t_material *local_material()
{
    t_material *m;

    m = alloc(sizeof(t_material), false);
    m->color = NULL;
    m->ambient = 0;
    m->diffuse = 0;
    m->specular = 0;
    m->shininess = 0;
    return m;
}



t_ray *ray_constrctor(t_tuple *origin, t_tuple *vect)
{
    t_ray *ray;

    ray = alloc(sizeof(t_ray), false);
    ray->point = *origin;
    ray->vect = *vect;
    return ray;
}

t_tuple *position(t_ray *ray, ld t)
{
    return (add_t(&ray->point, scalar_mult(&ray->vect, t)));
}
t_inters *intersection(ld p, t_sphere *obj)
{
    t_inters *inters;

    if (!obj)
        return NULL;
    inters = alloc(sizeof(t_inters), false);
    inters->sphere = obj;
    inters->inters_value = p;
    return (inters);
}

t_intersections *intersections(int count, t_inters **list_objs)
{   
    t_intersections *list;

    if(!list_objs || !*list_objs)
        return NULL;
    list = alloc(sizeof(t_intersections), false);
    list->count = count;
    list->inters_list = alloc(sizeof(t_inters *) * (count + 1), false);
    int i = 0;
    while (list_objs[i] && i < count)
    {
        list->inters_list[i] = alloc(sizeof(t_inters), false);
        list->inters_list[i]->sphere = list_objs[i]->sphere;
        list->inters_list[i]->inters_value = list_objs[i]->inters_value;
        i++;
    }
    list->inters_list[i] = NULL;
    return list;
}

t_ray *transform(t_ray *ray, t_matrix *m);

t_intersections *intersect(t_ray *ray, t_sphere *s)
{
    // t_matrix *inv = matrix_inverte(s->transform);
    t_matrix *inv = s->inv;
    t_ray *local_ray = transform(ray, inv);
    // t_ray *local_ray = ray;
    t_intersections *res;
    t_inters **inters;
    ld a, b, c, delta;
    // ld *in;
    int count;
    if (!ray || !s || !inv)
        return NULL;
    res = alloc(sizeof(t_intersections), false);
    a = dot_product(&local_ray->vect, &local_ray->vect);
    b = 2 * dot_product(&local_ray->point, &local_ray->vect);
    c = dot_product(&local_ray->point, &local_ray->point) - 1;
    delta = pow(b, 2) - 4 * a* c;

    if (delta < 0)
    {
        // printf("delta = %Lf || hello in here     || ", delta);
        inters = alloc(sizeof(t_inters *), false);
        *inters = NULL;
        count = 0;
    }
    else if (delta == 0)
    {
        count = 1;
        inters = alloc(sizeof(t_inters *), false);
        *inters = alloc(sizeof(t_inters), false);
        ld t;
        t = (-b) / (2 * a);
        (*inters)->inters_value = t;
        (*inters)->sphere = s;
    }
    else
    {
        count = 2;
        inters = alloc(sizeof(t_inters *)* 3, false);
        ld t1,t2;
        t1 = ((-1 * b) - sqrtl(delta)) / (2 * a);
        t2 = ((-1 * b) + sqrtl(delta)) / (2 * a);
        int i = 0;
        while (i < count)
            inters[i++] = alloc(sizeof(t_inters), false);
        inters[i] = NULL;
        inters[0]->inters_value = t1;
        inters[1]->inters_value = t2;
        inters[0]->sphere = inters[1]->sphere = s;
    }
    res->inters_list = inters;
    res->count = count;
    return res;
}

t_inters *hit(t_intersections *inters)
{
    t_inters *hit = NULL;
    // if(!inters)
    //     printf("inters");
    // if(!inters->inters_list)
    //     printf("inters_lis");
    // if(!*inters->inters_list)
    //     printf("fisrt defrins of inters_lis");
    if (!inters || !inters->inters_list || !*inters->inters_list)
        return NULL;

    for (int i = 0; inters->inters_list[i]; i++)
    {
        ld t = inters->inters_list[i]->inters_value;
        if (t >= 0 && (!hit || t < hit->inters_value))
            hit = inters->inters_list[i];
    }
    return hit;
}

t_ray *transform(t_ray *ray, t_matrix *m)
{
    t_ray *new_ray;

    if (!ray || !m)
        return NULL;
    new_ray = alloc(sizeof(t_ray), false);
    new_ray->point = *matrix_to_tuple(matrix_tuple(m, &ray->point));
    new_ray->vect = *matrix_to_tuple(matrix_tuple(m, &ray->vect));
    return new_ray;
}

void set_transform(t_sphere **s, t_matrix *transformation)
{
    if (!s || !*s || !transformation)
        return ;
    (*s)->transform =  transformation;
    return ;
}

// int main()
// {
//     t_ray *ray;
//     t_sphere *s;
//     ld p[] = {0, 0, -5};
//     ld v[] = {0, 0, 1};
//     ray = ray_constrctor(create_point(p), create_vector(v));
//     s = sphere();
//     set_transform(&s, translation(5, 0, 0));
//     t_intersections *xs = intersect(ray, s);
//     printf("count = %d\n", xs->count);
//     for(int i = 0; xs->inters_list[i]; i++)
//         printf("%Lf \n", xs->inters_list[i]->inters_value);
// }