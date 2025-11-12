#ifndef STACK_H
#define STACK_H


#include
t_tuple s_create_tuple(ld *coord, int type);
t_ray s_ray_constrctor(t_tuple *origin, t_tuple *vect);
t_ray s_transform(t_ray *ray, t_matrix *m);
t_tuple s_matrix_to_tuple(t_matrix *m);


#endif