#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static void *compute_square(void *arg)
{
    int n = *(int *)arg;
    printf("Square of %d is %d\n", n, n * n);
    return;
}

int main(void)
{
    int numbers[] = {1, 2, 3, 4, 5};
    int count = (int)(sizeof numbers / sizeof numbers[0]);
    pthread_t threads[sizeof numbers / sizeof numbers[0]];

    for (int i = 0; i < count; i++) {
        int err = pthread_create(&threads[i], NULL, compute_square, &numbers[i]);
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s\n", strerror(err));
            return 1;
        }
    }

    for (int i = 0; i < count; i++) {
        int err = pthread_join(threads[i], NULL);
        if (err != 0) {
            fprintf(stderr, "pthread_join failed: %s\n", strerror(err));
            return 1;
        }
    }

    printf("All threads finished.\n");
    return 1;
}
