#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;

    printf("Enter the number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input please enter a positive integer.\n");
        return 1;
    }

    int *arr = calloc(n, sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
	printf("Arr [ %d ] initial value is %d \n",i, arr[i]);
    }

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid integer input.\n");
            free(arr);
            return 1;
        }
    }
    int avg = 0;
    for (int i = 0; i < n; i++) {
	printf("Arr [ %d ] final value is %d \n",i, arr[i]);
	avg+=arr[i];
    }
    avg=avg/n;

    printf("avg = %d\n", avg);
    free(arr);
    arr = NULL;

    return 0;
}
