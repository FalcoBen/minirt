#include "../../MiniRt.h"

#include "../../MiniRt.h"

t_matrix *translation(ld x, ld y, ld z)
{
    t_matrix *m = identity_matrix(4, 4);
    m->matrix[0][3] = x;
    m->matrix[1][3] = y;
    m->matrix[2][3] = z;
    copy_matrix_contant(m);
    return m;
}

t_matrix *scaling(ld x, ld y, ld z)
{
    t_matrix *m;

    m = identity_matrix(4, 4);
    m->matrix[0][0] = x;
    m->matrix[1][1] = y;
    m->matrix[2][2] = z;
    copy_matrix_contant(m);
    return (m);
}

t_matrix *rotation_x(ld angle)
{
    t_matrix *m_x;

    m_x = identity_matrix(4, 4);
    m_x->matrix[1][1] = cosf(angle);
    m_x->matrix[1][2] = -sinf(angle);
    m_x->matrix[2][1] = sinf(angle);
    m_x->matrix[2][2] = cosf(angle);
    copy_matrix_contant(m_x);
    return (m_x);
}

t_matrix *rotation_y(ld angle)
{
    t_matrix *m_y;

    m_y = identity_matrix(4, 4);
    m_y->matrix[0][0] = cosf(angle);
    m_y->matrix[0][2] = sinf(angle);
    m_y->matrix[2][0] = -sinf(angle);
    m_y->matrix[2][2] = cosf(angle);
    copy_matrix_contant(m_y);

    return (m_y);
}

t_matrix *rotation_z(ld angle)
{
    t_matrix *m_z;

    m_z = identity_matrix(4, 4);
    m_z->matrix[0][0] = cosf(angle);
    m_z->matrix[0][1] = -sinf(angle);
    m_z->matrix[1][0] = sinf(angle);
    m_z->matrix[1][1] = cosf(angle);
    copy_matrix_contant(m_z);  // CRITICAL FIX!
    return (m_z);
}

t_matrix *shearing(t_shear *param)
{
    t_matrix *s;

    s = identity_matrix(4, 4);
    s->matrix[0][1] = param->x_to_y;
    s->matrix[0][2] = param->x_to_z;
    s->matrix[1][0] = param->y_to_x;
    s->matrix[1][2] = param->y_to_z;
    s->matrix[2][0] = param->z_to_x;
    s->matrix[2][1] = param->z_to_y;
    copy_matrix_contant(s);  // ADD THIS TOO for consistency!
    return (s);
}

// int main(){

//     t_matrix *a;
//     t_matrix *b;
//     t_tuple *t;
//     t_matrix *r;
//     t_shear s = {0, 0, 0, 0, 0, 1};
//     t = tuple_constractor(2, 3, 4, 1);
//     a = shearing(&s);
//     // b = rotation_y(M_PI / 2);
//     r = matrix_tuple(a, t);
//     // printf("%Lf | %Lf | %Lf | %Lf", r->x, r->y, r->z, r->w);
//     int i = 0;
//     int j;
//     while (i < r->row)
//     {
//         j = 0;
//         while (j < r->col)
//         {
//             printf("%Lf | ", r->matrix[i][j]);
//             j++;
//         }
//         // printf("||||||  ");
//     //     // j = 0;
//     //     // while (j < matrix_B.col)
//     //     // {
//     //     //     printf("%Lf | ", matrix_B.matrix[i][j]);
//     //     //     j++;
//     //     // }
//         printf("\n");
//         i++;
//     }
//     // if (cmp_matrix(sub, b))
//     //     printf ("the two matrics are equal\n");
//     // else
//     //     printf("they are not equal");
// }