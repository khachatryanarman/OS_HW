#include <stdio.h> 
#include <stdlib.h>

int main () {

	int* arr = (int*)malloc(sizeof(int)*10);
	if (arr == NULL) {
		printf("Malloc has failed\n");
		return 1;
	}
	
	int n = 10;
    	printf("Enter %d integers:\n", n);
    	for (int i = 0; i < n; i++) {
    	    if (scanf("%d", &arr[i]) != 1) {
    	        printf("Invalid integer input.\n");
    	        free(arr);
    	        return 1;
    	    }
    	}

	int* tmp = realloc (arr, 5 * sizeof(int));
	if (tmp == NULL) {
		printf("Realloc has failed\n");
		free(arr);
		return 1;
	}
	
	arr = tmp;

	for (int i = 0; i < 5; ++i) {
		printf("reallocated arr %d \n", arr[i]);
	}

	free(arr);
	return 0;

}
