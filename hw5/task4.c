#include <stdio.h>
#include <stdlib.h>



int main () {

	char** arr = malloc(sizeof(char*) * 3);
	if (arr == NULL) {
        	printf("Memory allocation failed\n");
        	return 1;
	}
	
	for (int i = 0; i < 3; ++i) {
		arr[i] = malloc(sizeof(char)*50);
		if(arr[i] == NULL) {
        		printf("Memory allocation failed\n");
        		return 1;
		}
		printf("Enter string %d (single word, max 49 chars): ", i + 1);
        	// %49s to leave room for '\0'
        	scanf("%49s", arr[i]);
	}

	for (int i = 0; i < 3; ++i) {
		printf("Strings %s \n", arr[i]);
	}


	char** tmp = realloc(arr, 5 * sizeof(char*));
	if (tmp == NULL) {
		printf ("Realloc failed \n");
		for (int i = 0; i < 3; ++i)
			free(arr[i]);
		free(arr);
		return 1;
	}
	arr = tmp;

	for (int i = 3; i < 5; ++i) {
		arr[i] = malloc(sizeof(char)*50);
		if(arr[i] == NULL) {
        		printf("Memory allocation failed\n");
			for (int i = 0; i < 3; ++i)
				free(arr[i]);
			free(arr);
        		return 1;
		}
		printf("Enter string %d (single word, max 49 chars): ", i + 1);
        	scanf("%49s", arr[i]);
	}

	for (int i = 0; i < 5; ++i) {
		printf("Strings after realloc  %s \n", arr[i]);
	}
		
	for (int i = 0; i < 5; ++i) {
		free(arr[i]);
	}

	free(arr);
		

	return 0;
}
