#include "../../MiniRt.h"

t_tuple *tuple_constractor(ld x, ld y,
    ld z, ld w)
{
    t_tuple *t;

    t = alloc(sizeof(t_tuple), false);
    t->w = w;
    t->x = x;
    t->y = y;
    t->z = z;
    return t;
}


/*
* if t1 == t2 : cmp_tuple return true 
* if t1 != t2 : cmp_tuple return flase
*/
bool cmp_tuple(t_tuple *t1, t_tuple *t2)
{
    if (!t1 || !t2)
        return false;
    if (t1->x == t2->x
        && t1->y == t2->y
        && t1->z == t2->z
        && t1->w == t2->w)
        return (true);
    return (false);
}

t_tuple *add_t(t_tuple *t1, t_tuple *t2)
{
    t_tuple *sum;

    if (!t1 || !t2)
        return (NULL);
    if (t1->w && t2->w)
        return (NULL);
    sum = alloc(sizeof(t_tuple), false);
    sum->x = t1->x + t2->x;
    sum->y = t1->y + t2->y;
    sum->z = t1->z + t2->z;
    sum->w = t1->w + t2->w;
    return (sum);
}

t_tuple *sub_t(t_tuple *t1, t_tuple *t2)
{
    t_tuple *sub;

    if (!t1 || !t2)
        return (NULL);
    if (!t1->w && t2->w)
        return (NULL);
    sub = alloc(sizeof(t_tuple), false);
    sub->x = t1->x - t2->x;
    sub->y = t1->y - t2->y;
    sub->z = t1->z - t2->z;
    sub->w = 0;
    return (sub);
}
t_tuple *neg_t(t_tuple *t)
{
    t_tuple *neg;

    if (!t)
        return (NULL);
    neg = alloc(sizeof(t_tuple), false);
    neg->x = -(t->x);
    neg->y = -(t->y);
    neg->z = -(t->z);
    neg->w = 0;
    return (neg);
}

t_tuple *scalar_mult(t_tuple *t , ld scalar)
{
    t_tuple *new;

    if (!t)
        return (NULL);
    new = alloc(sizeof(t_tuple), false);
    new->x = t->x * scalar;
    new->y = t->y * scalar;
    new->z = t->z * scalar;
    new->w = t->w * scalar;
    return (new);
}

ld magn(t_tuple *t)
{
    if (!t || t->w)
        return (-1);
    return (sqrtf(powf(fabsf(t->x), 2) + powf(fabsf(t->y), 2) + powf(fabsf(t->z), 2)));
}

t_tuple *vec_norm(t_tuple *t)
{
    t_tuple *norm;

    if (!t)
        return (NULL);
    norm = alloc(sizeof(t_tuple), false);
    norm->x = t->x / magn(t);
    norm->y = t->y / magn(t);
    norm->z = t->z / magn(t);
    norm->w = 0;
    return (norm);
}
ld dot_product(t_tuple *t1, t_tuple *t2)
{
    ld dot_res;

    if (!t1 || !t2)
        return (-1);
    dot_res = (t1->x * t2->x) + (t1->y * t2->y) + (t1->z * t2->z);
    return (dot_res);
}

t_tuple *cross_product(t_tuple *t1, t_tuple *t2)
{
    t_tuple *cross;

    if (!t1 || !t2 || t1->w || t2->w)
        return (NULL);
    cross = alloc(sizeof(t_tuple), false);
    cross->x = (t1->y * t2->z) - (t1->z * t2->y);
    cross->y = (t1->z * t2->x) - (t1->x * t2->z);
    cross->z = (t1->x * t2->y) - (t1->y * t2->x);
    cross->w = 0;
    return (cross);
}
// int main(){
//     t_tuple t1 = {2, 3, 4, 0};
//     t_tuple t2 = {1, 2, 3, 0};

//     t_tuple *sum = cross_product(&t1, &t2);
//     printf("dot product = {%Lf, %Lf, %Lf, %Lf}\n", sum->x, sum->y, sum->z, sum->w);
//     // printf("dot product = %Lf\n", dot_product(&t2, &t1));

// }