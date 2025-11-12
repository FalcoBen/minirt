#include "../../MiniRt.h"



t_color *color_op(t_color *c1, t_color *c2, char op)
{
    t_color *new;

    if (!c1 || !c2)
        return (NULL);
    new = alloc(sizeof(t_color), false);
    if (op == '+')
    {
        new->r = c1->r + c2->r;
        new->g = c1->g + c2->g;
        new->b = c1->b + c2->b;
    }
    else if (op == '-')
    {
        new->r = c1->r - c2->r;
        new->g = c1->g - c2->g;
        new->b = c1->b - c2->b;
    }
    else if (op == '*')
    {
        new->r = c1->r * c2->r;
        new->g = c1->g * c2->g;
        new->b = c1->b * c2->b;
    }
    return (new);
}

t_color *color_scalar(t_color *c, double scalar)
{
    t_color *new;

    if (!c)
        return (NULL);
    new = alloc(sizeof(t_color), false);
    new->r = c->r * scalar;
    new->g = c->g * scalar;
    new->b = c->b * scalar;
    return (new);
}

// int main(){
//     t_color color1 = {1, 0.2, 0.4};
//     t_color color2 = {0.9, 1, 0.1};

//     t_color *new = color_op(&color1, &color2, '*');
//     // t_color *new = color_scalar(&color2, 2);
//     printf("color : {r = %f , g = %f , b = %f}\n", new->r, new->g, new->b);

//     return 0;
// }