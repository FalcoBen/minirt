#include "../../MiniRt.h"


// function view_transform(from, to, up)
// forward ← normalize(to - from)
// upn ← normalize(up)
// left ← cross(forward, upn)
// true_up ← cross(left, forward)
// orientation ← matrix( left.x, left.y, left.z, 0,
// true_up.x, true_up.y, true_up.z, 0,
// -forward.x, -forward.y, -forward.z, 0,
// 0, 0, 0, 1)
// return orientation * translation(-from.x, -from.y, -from.z)
// end function

t_matrix *view_transformation(t_tuple *from, t_tuple *to, t_tuple *up)
{
    t_matrix *view;
    t_tuple *forward = vec_norm(sub_t(to, from));
    t_tuple *upn = vec_norm(up);
    t_tuple *left = cross_product(forward, upn);
    t_tuple *true_up = cross_product(left, forward);
    ld arr[16] = {
    left->x,    left->y,    left->z,    0,
    true_up->x, true_up->y, true_up->z, 0,
    -forward->x, -forward->y, -forward->z, 0,
    0,          0,          0,          1 };

    view = matrix_constractor(4, 4, arr, 16);
    view = matrix_multi(view, translation(-from->x, -from->y, -from->z));
    return view;
}


// int main()
// {
//     t_tuple *form = tuple_constractor(1, 3, 2, 1);
//     t_tuple *to = tuple_constractor(4, -2, 8, 1);
//     t_tuple *up = tuple_constractor(1, 1, 0, 0);
//     t_matrix *view = view_transformation(form, to, up);
//     // if (cmp_matrix(view, translation(0, 0, -8)))
//     // {
//         for (int i = 0; i < view->row; i++) {
//             for(int j = 0; j < view->col; j++){
//                 printf("%Lf || ", view->matrix[i][j]);
//             }
//             printf("\n");
//         }
//     // }
//     // else
//     //     puts("fails\n");
// }