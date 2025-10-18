#include "minirt.h"

void test_vector_math(void)
{
	printf("=== VECTOR MATH TEST ===\n");
    
    t_vec3 a = {1, 2, 3};
    t_vec3 b = {4, 5, 6};
    
    t_vec3 add = vec_add(a, b);
    printf("Add: (%f, %f, %f)\n", add.x, add.y, add.z);
    
    t_vec3 sub = vec_sub(a, b);
    printf("Sub: (%f, %f, %f)\n", sub.x, sub.y, sub.z);
    
    double dot = vec_dot(a, b);
    printf("Dot: %f\n", dot);
    
    t_vec3 cross = vec_cross(a, b);
    printf("Cross: (%f, %f, %f)\n", cross.x, cross.y, cross.z);
    
    double len = vec_length(a);
    printf("Length a: %f\n", len);
    
    t_vec3 norm = vec_normalize(a);
    printf("Normalized a: (%f, %f, %f)\n", norm.x, norm.y, norm.z);
    printf("Length normalized: %f\n", vec_length(norm));
    printf("=========================\n");
}