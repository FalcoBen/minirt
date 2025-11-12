#include "../../MiniRt.h"


int r(int p, char c)
{
    if (c == 'x')
        return (p +(WIDTH / 2));
    else
        return ((HEIGHT / 2) - p);
}




// int main()
// {
//     printf("=======> (%d, %d)\n", r(0, 'x'), r(-250, 'y'));
// }