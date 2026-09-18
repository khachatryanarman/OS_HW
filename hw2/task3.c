#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main () {
	pid_t id = fork ();
	if (id < 0) {
        	printf("Error:  fork failed\n");
        	exit(1);
    	}
	if (id == 0) {
		int ret = execl("/bin/echo", "echo", "hello from the child process", NULL);
		if (ret == -1) printf ("Error: subprocess was not created\n");
		exit(1);
	} else {
		int status;
		wait(&status);
		printf ("Parent Process: process done\n");
	}	



	return 0;
} 
