#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3

void* f (void* arg) {
	int thread_num = *((int*)arg);
	pthread_t tid = pthread_self();
	printf("Thread %lu is running\n", tid);
	return NULL;
}

int main () {
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];

	for (int i=0; i<NUM_THREADS; ++i) {
		thread_args[i] = i + 1;
		if (pthread_create(&threads[i], NULL, f, &thread_args[i]) != 0) {
			printf("failed to create thread");
			return 1;
		}
	}

	for (int i=0; i<NUM_THREADS; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			printf("failed to join thread");
			return 1;
		}
	}

	printf("All threads finished");
	return 0;
}
