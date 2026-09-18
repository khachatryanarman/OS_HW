#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	pid_t pid1, pid2;
	pid1 = fork();
	if (pid1 < 0) {
        	printf("Error: First fork failed\n");
        	exit(1);
    	} else if (pid1 == 0) {
        	execl("/bin/ls", "ls", NULL);
        	printf("Error: execl failed for ls\n");
        	exit(1);
    	}
    	waitpid(pid1, NULL, 0);
    	pid2 = fork();

    	if (pid2 < 0) {
    	    printf("Error: Second fork failed\n");
    	    exit(1);
    	} else if (pid2 == 0) {
    	    execl("/bin/date", "date", NULL);
    	    printf("Error: execl failed for date\n");
    	    exit(1);
    	}

    	waitpid(pid2, NULL, 0);
    	printf("Parent process done\n");

    	return 0;
}
