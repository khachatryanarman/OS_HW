#include <stdio.h>
#include <pthread.h>

int arr[] = {1, 2, 3, 4, 5, 6, 7};

void* sum_half(void* arg) {
    int* data = (int*)arg;
    int half = data[0];
    int size = data[1];

    int mid = size / 2;
    int start = (half == 0) ? 0 : mid;
    int end   = (half == 0) ? mid : size;

    int sum = 0;
    for (int i = start; i < end; i++) {
        sum += arr[i];
    }

    printf("Partial sum (half %d): %d\n", half + 1, sum);
    return NULL;
}

int main() {
    int size = sizeof(arr) / sizeof(arr[0]);
    pthread_t t1, t2;

    int arg1[] = {0, size};
    int arg2[] = {1, size};

    pthread_create(&t1, NULL, sum_half, arg1);
    pthread_create(&t2, NULL, sum_half, arg2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

