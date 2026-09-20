#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	pid_t pid = fork();

	if (pid < 0) {
    		perror("Fork failed");
    		return 1;
	}
	else if (pid == 0) {
    		printf("Child Process (PID: %d) exiting\n", getpid());
    		exit(0);
	} 
	else {
    		printf("Parent Process (PID: %d) created Child (PID: %d).\n", getpid(), pid);
    		printf("Parent is sleeping for 30 seconds check 'ps' or 'top' now!\n");
		// wait(NULL);
    		sleep(30); 
    		printf("Parent process exiting.\n");
	}

	return 0;
}
