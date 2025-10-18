#include <stdio.h>
#include <stdlib.h>

double  switch_ld(int color)
{
    return (color/255.0);
}

int main()
{
    int r = 255;
    int g = 100;
    int b = 200;
    double ldr = switch_ld(r);
    double ldg = switch_ld(g);
    double ldb = switch_ld(b);
    printf("double r = %f\n", ldr);
    printf("double g = %f\n", ldg);
    printf("double b = %f\n", ldb);
}

/*
cc int_to_ld.c
➜  miniRT git:(master) ✗ ./a.out 
double r = 1.000000
double g = 0.392157
double b = 0.784314
*/