#include "../../MiniRt.h"

/*
* if a = b : float_cmp return true
* if a != b : float_cmp retur flase
*/
bool ldbl_cmp(ld a, ld b)
{
    ld diff = fabsl(a - b);
    ld largest = fmaxl(fabsl(a), fabsl(b));
    ld abs_epsilon = 1e-18L;  
    ld rel_epsilon = 1e-15L;  
    return diff <= fmaxl(abs_epsilon, largest * rel_epsilon);
}

// int main(){

//     ld a = 0.1;
//     ld b = 0.2;


//     if (ldbl_cmp((a+b), (ld)0.3))
//         printf("true\n");
//     else
//         printf("false\n");
// }