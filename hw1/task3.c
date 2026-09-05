#include <stdio.h>


void swap (int* a, int * b) {
    if (a == NULL || b == NULL) return;
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main () {
    int x = 8;
    int y = 9;

    printf("before swap: x %d, y %d \n", x, y); 
    swap(&x, &y);
    printf("after swap: x %d, y %d", x, y); 

    return 0;
}
