#include "../../MiniRt.h"


ld **grid(int row, int col, ld *arr)
{
    ld **grid;
    int i;
    int j;
    grid = alloc((sizeof(ld *) * row), false);
    i = 0;
    while (i < row)
    {
        grid[i] = alloc(sizeof(ld) * col, false);
        for (j = 0; j < col; j++)
        {
            if (!arr)
                grid[i][j] = 0;
            else
                grid[i][j] = arr[i * col + j];
        }
        i++;
    }
    return (grid);
}
t_matrix *matrix_constractor(int row, int col, ld *arr, size_t arr_s)
{
    t_matrix *matrix;

    if (arr && arr_s != (size_t)(row * col))
        return NULL;
    matrix = alloc(sizeof(t_matrix), false);
    matrix->row = row;
    matrix->col = col;
    matrix->matrix = grid(row, col, arr);
    memset(matrix->stack_matrix, 0, sizeof(matrix->stack_matrix));
    return (matrix);
}

t_matrix *identity_matrix(int row, int col)
{
    t_matrix *id;
    int tag = 0;
    int i = 0;
    int j = 0;

    if (row != col)
        return (NULL);
    id = matrix_constractor(row, col, NULL, (size_t)NULL);
    while (i < row)
    {
        j = 0;
        while (j < col)
        {
            if (i == tag && j == tag)
            {
                id->matrix[i][j] = 1;
                tag++;
            }
            j++;
        }
        i++;
    }
    return (id);
}
// t_matrix *identity_matrix(int row, int col)
// {
//     t_matrix *m = alloc(sizeof(t_matrix), false);
//     m->row = row;
//     m->col = col;
//     m->matrix = alloc(sizeof(ld *) * row, false);
//     for (int i = 0; i < row; i++)
//     {
//         m->matrix[i] = alloc(sizeof(ld) * col, false);
//         for (int j = 0; j < col; j++)
//             m->matrix[i][j] = (i == j) ? 1.0 : 0.0;
//         for (int j = 0; j < col; j++)
//             m->stack_matrix[i][j] = m->matrix[i][j];
//     }
//     return m;
// }


bool cmp_matrix(t_matrix *m1, t_matrix *m2)
{
    int i;
    int j;

    if (!m1 || !m2 || !m1->matrix || !m2->matrix)
        return (NULL);
    i = 0;
    if ((m1->row != m2->row) || (m1->col != m2->col))
        return (false);
    while (i < m1->row)
    {
        j = 0;
        while (j < m1->col)
        {
            if (m1->matrix[i][j] != m2->matrix[i][j])
                return (false);
            j++;
        }
        i++;
    }
    return (true);
}

t_matrix *matrix_multi(t_matrix *m1, t_matrix *m2)
{
    t_matrix *res;
    if (!m1 || !m2 || m1->col != m2->row)
        return (NULL);
    int v_size = m1->row * m2->col;
    ld *va_list = alloc(sizeof(ld) * v_size, false);
    // int v = 0;
    int i, j;
    i = j = 0;
    for (i = 0; i < m1->row; i++)
    {
        for (j = 0; j < m2->col; j++)
        {
            va_list[i * m2->col + j] = 0;
            for (int k = 0; k < m1->col; k++)
                va_list[i * m2->col + j] += m1->matrix[i][k] * m2->matrix[k][j];
        }
    }
    res = matrix_constractor(m1->row, m2->col, va_list, v_size);

    copy_matrix_contant(res);
    return res;
}

/*
*multiply a matrix by a tuple
*/
t_matrix *matrix_tuple(t_matrix *m, t_tuple *t)
{
    ld va_list[4];
    t_matrix tuple_matrix;
    t_matrix *res;

    va_list[0] = t->x;
    va_list[1] = t->y;
    va_list[2] = t->z;
    va_list[3] = t->w;

    tuple_matrix.row = 4;
    tuple_matrix.col = 1;
    tuple_matrix = *matrix_constractor(4, 1, va_list, 4);
    res = matrix_multi(m, &tuple_matrix);
    return res;
}

t_matrix *matrix_transpos(t_matrix *m)
{
    t_matrix *trans;
    // ld *va_list;
    int i = 0;
    int j;

    if (!m)
        return (NULL);
    trans = matrix_constractor(m->row, m->col, NULL, (size_t)NULL);
    while (i < m->row)
    {
        j = 0;
        while (j < m->col)
        {
            trans->matrix[j][i] = m->matrix[i][j];
            j++;
        }
        i++;
    }
    copy_matrix_contant(trans);
    return (trans);
}
t_tuple *matrix_to_tuple(t_matrix *m)
{
    t_tuple *t;
    if (m->row != 4 || m->col != 1)
        return NULL;
    t = alloc(sizeof(t_tuple), false);
    t->x = m->matrix[0][0];
    t->y = m->matrix[1][0];
    t->z = m->matrix[2][0];
    t->w = m->matrix[3][0];
    return t;
}

// And the following matrix A:
//  1 , 2 , 3 , 4 ,
//  5 , 6 , 7 , 8 ,
//  9 , 8 , 7 , 6 ,
//  5 , 4 , 3 , 2 
// And the following matrix B:
//  -2 , 1 , 2 , 3 ,
//  3 , 2 , 1 , -1 ,
//  4 , 3 , 6 , 5 ,
//  1 , 2 , 7 , 8 
// Then A * B is the following 4x4 matrix:
    // 20, 22 , 50 , 48 ,
    // 44, 54 , 114 , 108 ,
    // 40, 58 , 110 , 102 ,
    // 16, 26 , 46 , 42 
//  1 , 2 , 3 , 4 ,
//  2 , 4 , 4 , 2 ,
//  8 , 6 , 4 , 1 ,
//  0 , 0 , 0 , 1 
/*****************************/
//  0,  9,  3,  0,
//  9,  8,  0,  8,
//  1,  8,  5,  3,
//  0,  0,  5,  8
// Then transpose(A) is the following matrix:
//  0,  9,  1,  0,
//  9,  8,  8,  0,
//  3,  0,  5,  5,
//  0,  8,  3,  8

// int main(){
//     t_matrix matrix_A;
//     t_matrix matrix_B;
//     t_matrix matrix_C;
//     t_matrix matrix_R;
//     // t_tuple t = {1, 2, 3, 1};
//     // matrix_A->row = matrix_B.row = matrix_C.row = 4;
//     // matrix_A->col = matrix_B.col = matrix_C.col = 4;
//     ld A[] = {
//         0,   1, 0, 1,
//         -250, -250, -249, -249,
//         0, 0, 0, 0,
//         1, 1, 1, 1,
//     };
//     ld val_B[] =
//     {
//         0,  9,  1,  0,
//         9,  8,  8,  0,
//         3,  0,  5,  5,
//         0,  8,  3,  8
//     };
//     ld val_C[] =
//     {
//         20, 22 , 50 , 48 ,
//         44, 54 , 114 , 108 ,
//         40, 58 , 110 , 102 ,
//         16, 26 , 46 , 42 
//     };
//     matrix_A = *matrix_constractor(4, 4, A, (sizeof(A)/sizeof(ld)));
//     // matrix_B = *matrix_constractor(matrix_B.row, matrix_B.col, val_B);
//     // matrix_C = *matrix_constractor(matrix_C.row, matrix_C.col, val_C);
//     // matrix_C = *identity_matrix(4, 4);
//     // matrix_R = *matrix_multi(&matrix_A, &matrix_B);
//     // matrix_C = *identity_matrix(4, 4);
//     // matrix_R = *matrix_transpos(&matrix_C);
//     int i = 0;
//     int j;
//     while (i < matrix_A.row)
//     {
//         j = 0;
//         while (j < matrix_A.col)
//         {
//             printf("%Lf | ", matrix_A.matrix[i][j]);
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
//     if (cmp_matrix(&matrix_R, &matrix_C))
//         printf ("the two matrics are equal\n");
//     else
//         printf("they are not equal");
// }