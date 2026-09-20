#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t id = fork();

	if (id < 0) {
        	printf("Error: subprocess was not created\n");
        	return 1;
    	}
    
	if (id == 0) {
        	printf("Child 1: id is %d\n", getpid());
        	exit(1);
   	} else { 
        	pid_t parentid = getpid();
        	wait(NULL);
        	printf("Parent: id is %d\n", parentid);
        	printf("Parent: waited until child %d is done\n", id); 
    	}

    	pid_t id2 = fork();

    	if (id2 < 0) {
       		printf("Error: subprocess was not created\n");
        	return 1;
    	}

    	if (id2 == 0) {
        	printf("Child 2: id is %d\n", getpid());
        	exit(1);
    	} else {
        	pid_t parentid = getpid();
        	int status;
        	waitpid(id2, &status, 0); 
        	printf("Parent: id is %d\n", parentid);
           	printf("Parent: waited until child %d is done with exit status %d\n", id2, status); 
    	}

    	return 0;
}
