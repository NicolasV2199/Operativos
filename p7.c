#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main( void ) {
	int i;
	pid_t root = getpid(); 
	if(!fork()){
		for(i=0; i<3; i++)
	    	if( !fork() )	    		
	    		break;	    	
				
		if(i==3){
			for(int j=0; j<3; j++){
	        	wait(NULL);
	        }
	    }
	}
	else{ 
		sleep(1);
		showtree();
		wait(NULL);
	    }
	return EXIT_SUCCESS;
}