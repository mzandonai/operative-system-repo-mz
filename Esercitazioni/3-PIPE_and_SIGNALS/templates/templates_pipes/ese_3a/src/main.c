#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "errExit.h"

#define PIPE_SYMBOL "|"

int main (int argc, char *argv[]) {
	
    int fd[2];
    if ( pipe(fd) == -1 ) {
		errExit("creating pipe");
    }
	
    pid_t pid1, pid2;
     
	/* 1st child process executes the first command */
    if ( (pid1 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid1 == 0 ) {
		...		
    }
	
	/* 2nd child process executes the first command */
    if ( (pid2 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid2 == 0 ) {
		...		
    }
	
	close(fd[0]);
	close(fd[1]);
	
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	
	return 0;
}