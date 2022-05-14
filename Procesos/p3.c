#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main( void ) {
	pid_t pidchild = fork();
	switch (pidchild)
	{ 
        case -1:
          perror("Error fork"); 
          exit(EXIT_FAILURE);
        case 0: 
          usleep(100000);
          printf( "P. hijo:  pid=%d y ppid=%d\n", getpid(), getppid() );
          break;
        default:
          printf( "P. padre: pid=%d y ppid=%d\n", getpid(), getppid() ); 
          wait(NULL); 
	}
	return EXIT_SUCCESS;
}