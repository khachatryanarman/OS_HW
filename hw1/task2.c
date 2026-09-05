#include <stdio.h>


int main () {
    int arr[5] = {4,3,2,1,0};
    int* parr = &arr[0];
    
    printf ("______________________________\n");
    for (int i = 0; i < 5; ++i) { printf ("arr %d = %d \n", i, *(parr + i)); }
    for (int i = 0; i < 5; ++i) { *(parr + i) = i; }
    printf ("______________________________\n");
    for (int i = 0; i < 5; ++i) { 
        printf ("arr %d = %d \n", i, arr[i]);
        printf ("arr %d = %d \n", i, *(parr + i));
    }


    return 0;
}
