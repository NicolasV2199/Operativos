#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main( void ) {
	pid_t root = getpid(); 

	for(int i=0; i<3; i++){
    	if( !fork() ){
    		if( i==0 || i==2){
    			fork();
    		}
    	    break;
    	}
	}
	if( root == getpid() ){ 
	    for(int j=0; j<3; j++){
	        wait(NULL);
	    }
	}
	else{
		wait(NULL);
	}
	
	return EXIT_SUCCESS;
}