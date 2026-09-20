#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


void print () {
	printf("From function print ()\n");
}

void print2 () {
	printf("From function print2 ()\n");
}


int main() {

	
		
	if (atexit(print) != 0) {
        	printf("Failed to register print\n");
       		return 1;
   	}
	if (atexit(print2) != 0) {
        	printf("Failed to register print2\n");
       		return 1;
   	}

	
	exit(1)	;

	return 0;
}
