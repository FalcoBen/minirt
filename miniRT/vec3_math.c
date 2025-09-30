#include "minirt.h"
#include <math.h>

t_vec3 vec_add(t_vec3 a, t_vec3 b)
{
    return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vec3 vec_sub(t_vec3 a, t_vec3 b)
{
    return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}

// multiply vector by scalar: v * scalar
t_vec3 vec_scale(t_vec3 v, double scalar)
{
    return ((t_vec3){v.x * scalar, v.y * scalar, v.z * scalar});
}

double vec_dot(t_vec3 a, t_vec3 b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3 vec_cross(t_vec3 a, t_vec3 b)
{
    return (
        (t_vec3){
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        }
    );
}

double vec_length(t_vec3 v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vec3 vec_normalize(t_vec3 v)
{
    double length;
    t_vec3 result;

    length = vec_length(v);
    if (length == 0.0)
        return ((t_vec3){0, 0, 0});
    
    result.x = v.x / length;
    result.y = v.y / length;
    result.z = v.z / length;
    return (result);
}

// Element-wise multiply: a * b (for color modulation)
t_vec3 vec_mult(t_vec3 a, t_vec3 b)
{
    return ((t_vec3){a.x * b.x, a.y * b.y, a.z * b.z});
}