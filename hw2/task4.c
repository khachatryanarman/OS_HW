#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main() {
	pid_t id = fork();

	if (id < 0) {
		printf("Error: fork failed\n");	
	} else if (id == 0) {
		int res = execl ("/bin/grep", "grep", "findSomething", "test.txt", NULL);
		if (res == -1) printf ("Error: subprocess was not created\n");
		exit(1);
	} else {
		int status;
		wait(&status);

		printf("Parent process: process completed\n");
	}




	return 0;
}
