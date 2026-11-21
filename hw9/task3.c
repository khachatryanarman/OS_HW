#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
#define COUNT 5

static void *print_numbers(void *arg)
{
    int num = *(int *)arg;
    pthread_t tid = pthread_self();

    for (int i = 1; i <= COUNT; i++) {
        printf("Thread %d (ID %lu): %d\n", num, (unsigned long)tid, i);
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        int err = pthread_create(&threads[i], NULL, print_numbers, &ids[i]);
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s\n", strerror(err));
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int err = pthread_join(threads[i], NULL);
        if (err != 0) {
            fprintf(stderr, "pthread_join failed: %s\n", strerror(err));
            return 1;
        }
    }

    printf("All threads finished.\n");
    return 0;
}
