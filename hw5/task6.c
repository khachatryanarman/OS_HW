#include <stdlib.h>
#include <string.h>

void* my_realloc(void* ptr, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    if (ptr == NULL) {
        return malloc(new_size);
    }

    void* new_ptr = malloc(new_size);
    if (new_ptr == NULL) {
        return NULL;
    }

    size_t copy_size = (old_size < new_size) ? old_size : new_size;
    memcpy(new_ptr, ptr, copy_size);
    free(ptr);

    return new_ptr;
}
int main () {
	int* arr = malloc(sizeof(int)*5);
	for(int i = 0; i < 5; ++i) {
		arr[i] = 0;
	}

	int* tmp = my_realloc(arr,5,3);
	if (tmp==NULL) return 1;

	arr = tmp;
	for(int i = 0; i < 3; ++i) {
		printf("arr val %d \n", arr[i]);
	}


	return 0;
}
