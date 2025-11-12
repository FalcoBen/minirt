#include "../../MiniRt.h"

// t_matrix *matrix_transpos(t_matrix *m)
// {
//     t_matrix *trans;
//     // ld *va_list;
//     int i = 0;
//     int j;

//     if (!m)
//         return (NULL);
//     trans = matrix_constractor(m->row, m->col, NULL, (size_t)NULL);
//     while (i < m->row)
//     {
//         j = 0;
//         while (j < m->col)
//         {
//             trans->matrix[j][i] = m->matrix[i][j];
//             j++;
//         }
//         i++;
//     }
//     return (trans);
// }


// t_matrix s_matrix_inverte(t_matrix *m)
// {
//     t_matrix invert;
//     t_matrix c;
//     int i = 0;
//     int j;
//     if (!m)
//         return (NULL);///////////

//     ld det = matrix_determinant(m);
//     // if (det == 0) 
//     //     return (NULL);


//     c = s_matrix_constractor(m->row, m->col, NULL);
//     while (i < m->row)
//     {
//         j = 0;
//         while (j < m->col)
//         {
//             c.matrix[i][j] = matrix_cofactor(m, i, j);
//             j++;
//         }
//         i++;
//     }

//     c = s_matrix_transpos(&c); /////////
//     invert = s_matrix_constractor(m->row, m->col, NULL);
//     i = 0;
//     while (i < m->row)
//     {
//         j = 0;
//         while (j < m->col)
//         {
//             invert.matrix[i][j] = (c.matrix[i][j] / det);
//             invert.stack_matrix[i][j] = (c.matrix[i][j] / det);
//             j++;
//         }
//         i++;
//     }
//     return invert;
// }