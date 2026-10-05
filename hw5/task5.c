#include <stdio.h>
#include <stdlib.h>

int main () {
	int num_students;

	printf("How many students are in the class? ");
	if (scanf("%d", &num_students) != 1 || num_students <= 0) {
		printf("Invalid number of students.\n");
		return 1;
	}

	int* grades = malloc(sizeof(int) * num_students);
	if (grades == NULL) {
		printf("Memory allocation failed\n");
		return 1;
	}
	
	for (int i = 0; i < num_students; ++i) {
		printf("Enter grade for student %d (0 to 100): ", i + 1);
		scanf("%d", &grades[i]);
		if (grades[i] < 0 || grades[i] > 100) {
			printf("Invalid grade! Grade must be between 0 and 100.\n");
			--i; 
		}
	}

	int highest = grades[0];
	int lowest = grades[0];

	for (int i = 1; i < num_students; ++i) {
		if (grades[i] > highest) {
			highest = grades[i];
		}
		if (grades[i] < lowest) {
			lowest = grades[i];
		}
	}

	printf("Highest Grade: %d\n", highest);
	printf("Lowest Grade: %d\n", lowest);

	free(grades);

	return 0;
}

