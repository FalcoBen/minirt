#include "../../MiniRt.h"

t_tuple s_cross_product(t_tuple *t1, t_tuple *t2)
{
    t_tuple cross = {0,0,0,0}; // Default to vector
    if (!t1 || !t2 || t1->w || t2->w) return cross;
    cross.x = (t1->y * t2->z) - (t1->z * t2->y);
    cross.y = (t1->z * t2->x) - (t1->x * t2->z);
    cross.z = (t1->x * t2->y) - (t1->y * t2->x);
    cross.w = 0;
    return cross;
}

// t_tuple s_cross_product(t_tuple *t1, t_tuple *t2) {
//     t_tuple cross = {0, 0, 0, 0};
//     if (!t1 || !t2 || t1->w || t2->w) return cross;
//     cross.x = (t1->z * t2->y) - (t1->y * t2->z);
//     cross.y = (t1->z * t2->x) - (t1->x * t2->z);
//     cross.z = (t1->x * t2->y) - (t1->y * t2->x);
//     cross.w = 0;
//     return cross;
// }






t_matrix view_transformation(t_tuple *from, t_tuple *to, t_tuple *up)
{
    t_matrix view = (t_matrix){0, 0, NULL, {{(ld){0}}}};
    
    t_tuple forward;
    forward = (t_tuple){0,0,0,0};
    
    s_sub_t(&forward, *to, *from);
    s_vec_norm(&forward);
    
    t_tuple upn = *up;
    upn.w = 0;
    s_vec_norm(&upn);
    
    t_tuple left = s_cross_product(&forward, &upn);
    t_tuple true_up = s_cross_product(&left, &forward);

    ld arr[16] = {
    left.x,    left.y,    left.z,    0,
    true_up.x, true_up.y, true_up.z, 0,
    -forward.x, -forward.y, -forward.z, 0,
    0,          0,          0,          1 };

    t_matrix tmp = s_matrix_constractor(4, 4, arr);
    view = s_matrix_multi(&tmp, translation(-from->x, -from->y, -from->z));
    return view;
}


// t_matrix view_transformation(t_tuple *from, t_tuple *to, t_tuple *up) {
//     t_tuple forward;
//     forward = (t_tuple){0,0,0}; 
//     s_sub_t(&forward, *to, *from);
//     s_vec_norm(&forward);
//     t_tuple upn = *up;
//     upn.w = 0;
//     s_vec_norm(&upn);
//     t_tuple left = s_cross_product(&forward, &upn);
//     t_tuple true_up = s_cross_product(&left, &forward);
//     ld arr[16] = {
//         left.x, left.y, left.z, 0,
//         true_up.x, true_up.y, true_up.z, 0,
//         -forward.x, -forward.y, -forward.z, 0,
//         0, 0, 0, 1
//     };
//     t_matrix orientation = s_matrix_constractor(4, 4, arr);
//     t_matrix *trans = translation(-from->x, -from->y, -from->z);
//     return s_matrix_multi(trans, &orientation);
// }
// t_matrix view_transformation(t_tuple *from, t_tuple *to, t_tuple *up) {
//     t_tuple forward;
//     forward = (t_tuple){0,0,0}; 
//     s_sub_t(&forward, *to, *from);
//     printf("forward: {%f, %f, %f, %f}\n", forward.x, forward.y, forward.z, forward.w);
//     s_vec_norm(&forward);
//     printf("normalized forward: {%f, %f, %f, %f}\n", forward.x, forward.y, forward.z, forward.w);

//     t_tuple upn = *up;
//     upn.w = 0;
//     s_vec_norm(&upn);
//     printf("normalized upn: {%f, %f, %f, %f}\n", upn.x, upn.y, upn.z, upn.w);

//     t_tuple left = s_cross_product(&forward, &upn);
//     printf("left: {%f, %f, %f, %f}\n", left.x, left.y, left.z, left.w);

//     t_tuple true_up = s_cross_product(&left, &forward); // Fix: Use left, forward
//     printf("true_up: {%f, %f, %f, %f}\n", true_up.x, true_up.y, true_up.z, true_up.w);

//     ld arr[16] = {
//         left.x, left.y, left.z, 0,
//         true_up.x, true_up.y, true_up.z, 0,
//         -forward.x, -forward.y, -forward.z, 0,
//         0, 0, 0, 1
//     };
//     t_matrix orientation = s_matrix_constractor(4, 4, arr);
//     printf("orientation matrix:\n");
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++)
//             printf("%f, ", arr[i * 4 + j]);
//         printf("\n");
//     }

//     t_matrix *trans = translation(-from->x, -from->y, -from->z);
//     printf("translation matrix:\n");
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++)
//             printf("%f, ", trans->stack_matrix[i][j]);
//         printf("\n");
//     }

//     t_matrix view = s_matrix_multi(&orientation, trans); // Fix: orientation * trans
//     printf("view matrix:\n");
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++)
//             printf("%f, ", view.stack_matrix[i][j]);
//         printf("\n");
//     }

//     return view;
// }