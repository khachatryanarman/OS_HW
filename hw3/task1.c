#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main() {

	
	pid_t id = fork ();

	if (id < 0) {
		printf ("Error: subprocess was not created\n");
	}
	if (id == 0) {
		pid_t id = getpid();
		printf ("Child: id is %d\n", id);
		
	} else {
		pid_t id = getpid();
		printf ("Parent: id is %d\n", id);
	}





	return 0;
}


