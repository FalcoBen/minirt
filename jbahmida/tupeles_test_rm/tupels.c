#include "../../MiniRt.h"

t_tuple *create_point(ld *coord)
{
    t_tuple *point;

    if (!coord)
        return (NULL);
    point = alloc(sizeof(t_tuple), false);
    point->x = coord[0];
    point->y = coord[1];
    point->z = coord[2];
    point->w = 1;
    // printf("%Lf || %Lf || %Lf \n", point->x, point->y, point->z);
    return (point);
}

t_tuple *create_vector(ld *coord)
{
    t_tuple *vector;

    if (!coord)
        return (NULL);
    vector = alloc(sizeof(t_tuple), false);
    vector->x = coord[0];
    vector->y = coord[1];
    vector->z = coord[2];
    vector->w = 0;
    return (vector);
}



// int main(){
//     t_tuple *p;
//     t_tuple *v;

//     ld *coor = alloc(sizeof(ld) * 3, false);
//     for(int i = 0; i < 3 ; i++)
//         coor[i] = i + rand();
//     p = create_point(coor);
//     for(int i = 0; i < 3 ; i++)
//         coor[i] = i + rand();
//     v = create_vector(coor);
//     printf("size of ld is : %lu\n", sizeof(ld));
//     printf("point cause : w is %Lf ||x= %Lf || y= %Lf || z= %Lf\n", p->w, p->x, p->y, p->z);
//     printf("vector cause : w is %Lf ||x= %Lf || y= %Lf || z= %Lf\n", v->w, v->x, v->y, v->z);
//     alloc(0, true);
// }