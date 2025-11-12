#include "../../MiniRt.h"

extern int g_fd;


// t_tuple s_sub_t(t_tuple t1, t_tuple t2)
// {
//     t_tuple local;

//     local.x = t1.x - t2.x;
//     local.y = t1.y - t2.y;
//     local.z = t1.z - t2.z;
//     local.w = 0;
//     return (local);
// }


// t_tuple s_vec_norm(t_tuple t)
// {
//     t_tuple local;
//     ld m = magn(&t);
//     local.x = t.x / m;
//     local.y = t.y / m;
//     local.z = t.z / m;
//     local.w = t.w;
//     return local;
// }

t_tuple s_create_tuple(ld *coord, int type)
{
    t_tuple local = {0, 0, 0, 0}; // Default w=0
    if (!coord) return local;
    local.x = coord[0];
    local.y = coord[1];
    local.z = coord[2];
    local.w = (type == 1) ? 1 : 0; // Point w=1, vector w=0
    return local;
}
/***************************************************************** */
t_stack_ray s_ray_constrctor(t_tuple *origin, t_tuple *vect)
{
    t_stack_ray ray;

    // if (!origin || !vect)
    //     return ray;
    ray.point = *origin;
    ray.vect = *vect;
    return ray;
}

t_stack_ray s_transform(t_stack_ray *ray, t_matrix *m)
{
    t_stack_ray local = (t_stack_ray){(t_tuple){0,0,0,1}, (t_tuple){0,0,0,0}};
    if (!ray || !m)
        return local;
    t_matrix m1 = s_matrix_tuple(m, &ray->point);
    t_tuple point = s_matrix_to_tuple1(&m1);
    t_matrix m2 = s_matrix_tuple(m, &ray->vect);
    t_tuple vect = s_matrix_to_tuple1(&m2);   
    local.point = point;
    local.vect = vect;
    return local;
} 

/******************************************************************* */
t_matrix  s_matrix_multi(t_matrix *m1, t_matrix *m2)
{
    t_matrix local;
    /*check the values of the matrix m2 cause it given from the call of the function translation*/
    if (m1->matrix)
        copy_matrix_contant(m1);
    if (m2->matrix)
        copy_matrix_contant(m2);
    local.col = -1;
    local.row = -1;
    if (!m1 || !m2 || m1->col != m2->row)
        return (local);
    int v_size = m1->row * m2->col;

    ld va_list[v_size];


    int i, j;
    i = j = 0;
    for (i = 0; i < m1->row; i++)
    {
        for (j = 0; j < m2->col; j++)
        {
            va_list[i * m2->col + j] = 0;
            for (int k = 0; k < m1->col; k++)
                va_list[i * m2->col + j] += m1->stack_matrix[i][k] * m2->stack_matrix[k][j];
        }
    }

    local = s_matrix_constractor(m1->row, m2->col, va_list);
    return local;
}


void s_grid(int row, int col, ld *arr, ld grid[4][4])
{
    int i;
    int j;
    i = 0;
    while (i < row)
    {
        for (j = 0; j < col; j++)
        {
            if (!arr)
                grid[i][j] = 0;
            else
                grid[i][j] = arr[i * col + j];
        }
        i++;
    }
    return ;
}
t_matrix s_matrix_constractor(int row, int col, ld *arr)
{
    t_matrix matrix;

    matrix.col = -1;
    matrix.row = -1;
    matrix.matrix = NULL;
    
    matrix.row = row;
    matrix.col = col;
    matrix.matrix = NULL;
    s_grid(row, col, arr, matrix.stack_matrix);
    return (matrix);
}

t_tuple s_matrix_to_tuple(t_matrix *m)
{
    t_tuple local;
    
    local = s_create_tuple(NULL, -1);
    if (m->row != 4 || m->col != 1)
        return local;
    local.x = m->stack_matrix[0][0];
    local.y = m->stack_matrix[1][0];
    local.z = m->stack_matrix[2][0];
    local.w = m->stack_matrix[3][0];
    return local;
}

t_tuple s_matrix_to_tuple1(t_matrix *m)
{
    t_tuple local;
    
    local = s_create_tuple(NULL, -1);
    if (m->row != 4 || m->col != 1)
    return local;
    local.x = m->stack_matrix[0][0];
    local.y = m->stack_matrix[1][0];
    local.z = m->stack_matrix[2][0];
    local.w = m->stack_matrix[3][0];
    return local;
}


// t_tuple s_position(t_stack_ray *ray, ld t)
// {
//     t_tuple p = {0,0,0,1};
//     t_tuple p1 = {0,0,0,1};
//     s_scalar_mult(&p1, ray->vect, t);
//     s_add_t(&p , ray->point, p1);
//     return (p);
// }

t_matrix s_matrix_tuple(t_matrix *m, t_tuple *t)
{
    t_matrix result = {4, 1, NULL, {{(ld){0}}}}; // matrix = NULL, use stack_matrix
    ld tuple_col[4] = {t->x, t->y, t->z, t->w}; // Column vector
    memset(result.stack_matrix, 0, sizeof(result.stack_matrix));

    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++) {
            result.stack_matrix[i][0] += m->stack_matrix[i][k] * tuple_col[k];
        }
    }
    return result;
}

// t_inters s_hit(t_stack_intersections inters)
// {
//     t_inters local;
//     local = (t_inters){0,NULL, NULL, NULL, NULL, -1, false};
    
//     if (!inters.existence || inters.count < 1)
//         return local;
//     for (int i = 0; i < inters.count; i++)
//     {
//         ld p = inters.world_inters_list[i].inters_value;
//         if (inters.world_inters_list[i].type == T_SPHERE)
//         {
//             if (!inters.world_inters_list[i].sphere)
//                 return local;
//         }
//         else if (inters.world_inters_list[i].type == T_CYLINDRE)
//         {
//             if (!inters.world_inters_list[i].cylinder)
//                 return local;
//         }
//         else if (inters.world_inters_list[i].type == T_CONE)
//         {
//             if (!inters.world_inters_list[i].cone)
//                 return local;
//         }
//         else if (inters.world_inters_list[i].type == T_PLAN)
//         {
//             if (!inters.world_inters_list[i].plane)
//                     return local;
//         }
//         if (p >= 0 && (p < local.inters_value))
//         {
//             local = inters.world_inters_list[i];
//             local.existence = true;
//         }
//     }
//     return local;
// } 
t_inters s_hit(t_stack_intersections inters)
{
    t_inters local;
    local = (t_inters){T_SPHERE, NULL, NULL, NULL, NULL, -1, false};
    
    if (!inters.existence || inters.count < 1)
        return local;
    
    ld min_t = INFINITY;
    bool found = false;
    
    for (int i = 0; i < inters.count; i++)
    {
        ld t = inters.world_inters_list[i].inters_value;
        
        // Skip negative t values (intersections behind the camera)
        if (t < 0)
            continue;
        
        // Check if this is a valid intersection based on type
        bool valid = false;
        if (inters.world_inters_list[i].type == T_SPHERE && inters.world_inters_list[i].sphere)
            valid = true;
        else if (inters.world_inters_list[i].type == T_CYLINDRE && inters.world_inters_list[i].cylinder)
            valid = true;
        else if (inters.world_inters_list[i].type == T_CONE && inters.world_inters_list[i].cone)
            valid = true;
        else if (inters.world_inters_list[i].type == T_PLAN && inters.world_inters_list[i].plane)
            valid = true;
        
        if (!valid)
            continue;
        if (t < min_t)
        {
            min_t = t;
            local = inters.world_inters_list[i];
            local.existence = true;
            found = true;
        }
    }
    
    if (!found)
        local.existence = false;
    
    return local;
}
/******************************************************************* */




// void s_intersect(t_stack_intersections *xs, t_stack_ray ray, t_sphere *s)
// {
//     ld a, b, c, delta;
//     t_stack_ray local_ray = s_transform(&ray, s->inv);
//     a = dot_product(&local_ray.vect, &local_ray.vect);
//     b = 2 * dot_product(&local_ray.point, &local_ray.vect);
//     c = dot_product(&local_ray.point, &local_ray.point) - 1;
//     delta = pow(b, 2) - 4 * a * c;
    
//     if (delta < 0)
//     {
//         xs->count = 0;
//         xs->existence = false;
//         xs->world_inters_list = NULL;
//         return;
//     }
    
//     xs->count = (delta > 0) ? 2 : 1;
//     xs->existence = xs->count > 0;    
//     if (xs->count == 1)
//     {
//         xs->inters_list[0].inters_value = (-b) / (2 * a);
//         xs->inters_list[0].object = s;
//         xs->inters_list[0].existence = true;
//     }
//     else if (xs->count == 2)
//     {
//         xs->inters_list[0].inters_value = ((-b) - sqrtl(delta)) / (2 * a);
//         xs->inters_list[1].inters_value = ((-b) + sqrtl(delta)) / (2 * a);
//         xs->inters_list[0].object = s;
//         xs->inters_list[1].object = s;
//         xs->inters_list[0].existence = true;
//         xs->inters_list[1].existence = true;
//     }
// }