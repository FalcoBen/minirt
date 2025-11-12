#include "../../MiniRt.h"
















// And cyl.maximum ← 2
// And direction ← normalize(<direction>)
// And r ← ray(<point>, direction)
// When xs ← local_intersect(cyl, r)
// Then xs.count = <count>
// Examples:
// | | point | direction | count |








void copy_matrix_contant(t_matrix *m)
{
    if (!m || !m->matrix)
        return;
    for (int i = 0; i < m->row; i++)
        for (int j = 0; j < m->col; j++)
            m->stack_matrix[i][j] = m->matrix[i][j];
    return;
}


int main()
{
    t_cylinder *cyl;
    cyl = malloc(sizeof(t_cylinder));
    cylinder(&cyl, NULL, false);
    cyl->minimum = 1;
    cyl->maximum = 2;
    cyl->closed = true;
    // Test case 1
    t_tuple *point = create_point((ld[3]){0, 3, 0});
    t_tuple *vect = create_vector((ld[3]){0, -1, 0});
    s_vec_norm(vect);
    t_stack_ray ray = s_ray_constrctor(point, vect);
    t_stack_intersections x;
    cylinder_intersct(&x, cyl, &ray);
    printf("the count is : %d\n", x.count);
    
    // Test case 2
    point = create_point((ld[3]){0, 3, -2});
    vect = create_vector((ld[3]){0, -1, 2});
    s_vec_norm(vect);
    ray = s_ray_constrctor(point, vect);
    cylinder_intersct(&x, cyl, &ray);
    printf("the count is : %d\n", x.count);
    
    // Test case 3 (corner case)
    point = create_point((ld[3]){0, 4, -2});
    vect = create_vector((ld[3]){0, -1, 1});
    s_vec_norm(vect);
    ray = s_ray_constrctor(point, vect);
    cylinder_intersct(&x, cyl, &ray);
    printf("the count is : %d\n", x.count);
    
    // Test case 4
    point = create_point((ld[3]){0, 0, -2});
    vect = create_vector((ld[3]){0, 1, 2});
    s_vec_norm(vect);
    ray = s_ray_constrctor(point, vect);
    cylinder_intersct(&x, cyl, &ray);
    printf("the count is : %d\n", x.count);
    
    // Test case 5 (corner case)
    point = create_point((ld[3]){0, -1, -2});
    vect = create_vector((ld[3]){0, 1, 1});
    s_vec_norm(vect);
    ray = s_ray_constrctor(point, vect);
    cylinder_intersct(&x, cyl, &ray);
    printf("the count is : %d\n", x.count);
    
    free(cyl);
}