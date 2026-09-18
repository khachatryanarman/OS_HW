#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main () {
	
	pid_t id = fork ();
	
	if (id < 0) {
		printf ("Error: subprocess was not created\n");
		return 1;
	}
	if (id == 0) {
		printf ("Child Process: started\n"); 
		int ret = execl ("/bin/ls", "ls", NULL);
		if (ret == -1) printf ("Error: subprocess was not created\n");
	       	exit (1);	
	} else {
		printf ("Parent Process: done\n");
	       	int status;
		wait(&status);
		printf ("Parent Process: child finished with status %d\n", status);
	}

	return 0;
}
