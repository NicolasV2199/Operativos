#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
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
                printf( "Proceso hijo:  pid=%d y ppid=%d\n", getpid(), getppid() ); 
		        break;
        default:
                printf( "Proceso padre: pid=%d y ppid=%d\n", getpid(), getppid() ); 
                
	}
	return 0;
}