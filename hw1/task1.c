#include <stdio.h>

int main () {
    int x = 0;
    int* px = &x;
    printf ("addr %p \n", px);
    printf ("addr %p \n", &x);
    
    *px = 5;
    printf ("value after modificaiton %d \n", x);
    



    return 0;
}
