#include <stdio.h>

int main () {
    int x = 0;
    int* px = &x;
    int** ppx = &px;

    printf ("x value by pointer to X %d\n", *px);
    printf ("x value by pointer to pointer to X %d\n", **ppx);
    



    return 0;
}
