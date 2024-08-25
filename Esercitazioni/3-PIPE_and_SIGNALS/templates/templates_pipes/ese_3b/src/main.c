#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "errExit.h"

#define PIPE_SYMBOL "|"

int main (int argc, char *argv[]) {
	
	int fd1[2];
	int fd2[2];
	
    pid_t pid1, pid2, pid3;
	
    if ( pipe(fd1) == -1 ) {
		errExit("creating pipe1");
    }
     
	/* 1st child process executes the first command */
    if ( (pid1 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid1 == 0 ) {
		...	
    }
	
    if ( pipe(fd2) == -1 ) {
		errExit("creating pipe1");
    }
	
	/* 2nd child process executes the first command */
    if ( (pid2 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid2 == 0 ) {
		...		
    }
	
	close(fd1[0]);
	close(fd1[1]);
	
	/* 3rd child process executes the first command */
    if ( (pid3 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid3 == 0 ) {
		...		
    }
	
	close(fd2[0]);
	close(fd2[1]);
	
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);
	
	return 0;
}