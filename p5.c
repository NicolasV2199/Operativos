#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main( void ) {
    
    int estado;
    pid_t pidchild, pidwait;
    
    pidchild = fork();
    
    switch (pidchild){
        case -1:
            perror("Error fork"); 
            exit(EXIT_FAILURE);
        case 0:
            //exit(100);
            return 125;
        default:
            pidwait = wait(&estado);
            if(pidwait != -1)
                if(WIFEXITED(estado))
                    printf("Hijo estado: %d\n", WEXITSTATUS(estado));
	}
	return EXIT_SUCCESS;
}