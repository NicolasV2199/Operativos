#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main( void ) {
	
	pid_t root = getpid(); 
	
	for(int i=0; i<3; i++){
    	if( !fork() ){
    	    break;
    	}
	}
	if( root == getpid() ){ 
	    for(int j=0; j<3; j++){
	        wait(NULL);
	    }
	    printf("Finalizando padre %d\n", getpid());
	}
	else{
	    printf("Finalizando hijo %d ppid:%d\n", getpid(), getppid());
	}
	
	return EXIT_SUCCESS;
}