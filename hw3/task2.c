#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t id1 = fork();

	if (id1 < 0) {
		printf("Error: subprocess was not created\n");
		return 1;
	}

	if (id1 == 0) {
		printf("Child 1: id is %d, exiting with 1\n", getpid());
		exit(1);
	}

	pid_t id2 = fork();

	if (id2 < 0) {
		printf("Error: subprocess was not created\n");
		return 1;
	}

	if (id2 == 0) {
		sleep(2);
		printf("Child 2: id is %d, exiting with 2\n", getpid());
		exit(2);
	}

	printf("Parent: id is %d\n", getpid());

	int status2;
	waitpid(id2, &status2, 0);
	if (WIFEXITED(status2))
		printf("Parent: waited for child %d, exit status %d\n", id2, WEXITSTATUS(status2));

	int status1;
	pid_t done = wait(&status1);
	if (WIFEXITED(status1))
		printf("Parent: waited for child %d, exit status %d\n", done, WEXITSTATUS(status1));

	return 0;
}
