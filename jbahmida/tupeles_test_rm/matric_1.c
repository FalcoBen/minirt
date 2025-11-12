#include "../../MiniRt.h"


// for( int i = 0; i < R - r + 1; i++){
//   for(int j = 0; j < C - c + 1; j++){
//     submatrix[i][j]=initial_matrix[i][j]
//   }
// }

ld det_small_matrix(t_matrix *m)
{
    ld det;
    ld **ptr;

    if (!m || m->row != 2 || m->col != 2)
        return (0);
    
    ptr = m->matrix;
    det = (ptr[0][0] * ptr[1][1]) - (ptr[1][0] * ptr[0][1]);
    return det;
}
t_matrix *submatrix(t_matrix *m, int r, int c)
{
    t_matrix *sub;
    ld *va_list;
    int va_size = (m->row - 1) * (m->col - 1);
    if (!m || va_size <= 0) return NULL;
    va_list = alloc(sizeof(ld) * va_size, false);
    int v = 0;
    for (int i = 0; i < m->row && v < va_size; i++)
        if (i != r)
            for (int j = 0; j < m->col && v < va_size; j++)
                if (j != c)
                    va_list[v++] = m->matrix[i][j];
    sub = matrix_constractor(m->row - 1, m->col - 1, va_list, va_size);
    return sub;
}

// static ld matrix_minor(t_matrix *m, int row, int col)
// {
//     // ld minor;

//     if (!m)
//         return (-1);
//     return det_small_matrix(submatrix(m, row, col));
// }
ld matrix_cofactor(t_matrix *m, int row, int col)
{
    t_matrix *minor;
    ld det;

    if (!m)
        return EIO;
    
    if(!m->matrix)
    {
        m->matrix = malloc(sizeof(ld *) * m->row);
        int i = 0;
        while (i < m->row)
        {
            m->matrix[i] = malloc(sizeof(ld) * m->col);
            for (int j = 0; j < m->col; j++)
                m->matrix[i][j] = m->stack_matrix[i][j];
            i++;
        }
    }
    minor = submatrix(m, row, col);
    det = matrix_determinant(minor);


    if ((row + col) % 2 == 0)
        return det;
    else
        return -det;
}


ld matrix_determinant(t_matrix *m)
{
    ld **ptr;
    ld det = 0;
    int c = 0;
    if (!m)
        return EIO;
    if(m->matrix)
        ptr = m->matrix;
    else
    {
        c = 1;
        ptr = malloc(sizeof(ld *) * m->row);
        int i = 0;
        while (i < m->row)
        {
            ptr[i] = malloc(sizeof(ld) * m->col);
            for (int j = 0; j < m->col; j++)
                ptr[i][j] = m->stack_matrix[i][j];
            i++;
        }
    }
    if (m->row == 2 && m->col == 2)
        det = det_small_matrix(m);
    else
    {
        for (int i = 0; i < m->col; i++)
            det += ptr[0][i] * matrix_cofactor(m, 0, i);
        if (c)
        {
            for(int i = 0; i < m->row; i++)
                free(ptr[i]);
            free(ptr);
        }
    }

    return det;
}

t_matrix *matrix_inverte(t_matrix *m)
{
    t_matrix *invert, *c;
    ld det = matrix_determinant(m);
    if (!m || fabs(det) < 0.0001) return NULL;
    c = matrix_constractor(m->row, m->col, NULL, (size_t)NULL);
    for (int i = 0; i < m->row; i++)
        for (int j = 0; j < m->col; j++)
            c->matrix[i][j] = matrix_cofactor(m, i, j);
    c = matrix_transpos(c);
    invert = matrix_constractor(m->row, m->col, NULL, (size_t)NULL);
    for (int i = 0; i < m->row; i++)
        for (int j = 0; j < m->col; j++)
            invert->matrix[i][j] = c->matrix[i][j] / det;
    copy_matrix_contant(invert); // Ensure stack_matrix is set
    return invert;
}

//  -6 , 1 , 1 , 6 ,
//  -8 , 5 , 8 , 6 ,
//  -1 , 0 , 8 , 2 ,
//  -7 , 1 , -1 , 1
/*************************/
//  3 , 5 , 0 ,
//  2 , -1 , -7 ,
//  6 , -1 , 5
/*************************/
// 1 , 2 , 6 ,
// -5 , 8 , -4 ,
// 2 , 6 , 4
/*************************/
// -2 , -8 , 3 , 5 ,
// -3 , 1 , 7 , 3 ,
// 1 , 2 , -9 , 6 ,
// -6 , 7 , 7 , -9
/*************************/
// 6 , 4 , 4 , 4 ,
// 5 , 5 , 7 , 6 ,
// 4 , -9 , 3 , -7 ,
// 9 , 1 , 7 , -6
/*************************/
// -5 , 2 , 6 , -8 ,
// 1 , -5 , 1 , 8 ,
// 7 , 7 , -6 , -7 ,
// 1 , -3 , 7 , 4
/*************************/
// 0.21805 , 0.45113 , 0.24060 , -0.04511 ,
// -0.80827 , -1.45677 , -0.44361 , 0.52068 ,
// -0.07895 , -0.22368 , -0.05263 , 0.19737 ,
// -0.52256 , -0.81391 , -0.30075 , 0.30639
/*************************/
//  8 , -5 , 9 , 2 ,
//  7 , 5 , 6 , 1 ,
//  -6 , 0 , 9 , 6 ,
//  -3 , 0 , -9 , -4 ,
/*************************/
//  3 , -9 , 7 , 3 ,
//  3 , -8 , 2 , -9 ,
//  -4 , 4 , 4 , 1 ,
//  -6 , 5 , -1 , 1
/*************************/
//  8 , 2 , 2 , 2 ,
//  3 , -1 , 7 , 0 ,
//  7 , 0 , 5 , 4 ,
//  6 , -2 , 0 , 5 ,

// int main(){

//     t_matrix *a;
//     a = identity_matrix(4, 4);
//     ld det = matrix_determinant(a);
//     printf("====> %Lf\n", det);
//     // printf("======> %Lf\n", matrix_cofactor(a, 0, 0));
//     // printf("======> %Lf\n", matrix_cofactor(a, 0, 1));
//     // printf("======> %Lf\n", matrix_cofactor(a, 0, 2));
//     // printf("======> %Lf\n", matrix_determinant(a));
//     // t_matrix *c = matrix_multi(b, a);
//     // t_matrix *sub = matrix_multi(c, matrix_inverte(a));
//     int i = 0;
//     int j;
//     while (i < a->row)
//     {
//         j = 0;
//         while (j < a->col)
//         {
//             printf("%Lf | ", a->matrix[i][j]);
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