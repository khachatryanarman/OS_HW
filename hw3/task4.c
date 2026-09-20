#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t id1 = fork();

	if (id1 < 0) {
		printf("Error: 1st fork failed\n");
		return 1;
	}

	if (id1 == 0) {
    		printf("Child 1 (PID: %d) exiting with error code 1\n", getpid());
    		exit(1);
	}	

	pid_t id2 = fork();

	if (id2 < 0) {
    		printf("Error: 2nd fork failed\n");
    		return 1;
	}

	if (id2 == 0) {
		printf("Child 2 (PID: %d) exiting with error code 2\n", getpid());
   		exit(2);
	}

	int status1, status2;
	waitpid(id1, &status1, 0);
	waitpid(id2, &status2, 0);

	if (WIFEXITED(status1) && WIFEXITED(status2)) {
		printf("Parent: Child 1 exit code %d Child 2 exit code %d\n",WEXITSTATUS(status1), WEXITSTATUS(status2));
	}

	return 0;
}
