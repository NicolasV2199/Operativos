#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void showtree(){
	char cmd[20]={""};
	sprintf(cmd,"pstree -cAlp %d", getpid());
	system(cmd);	
}

int main( void ) {
	int i;
	pid_t root = getpid(), out; 
	if(!fork()){
		for(i=0; i<3; i++)
	    	if( !fork() )	    		
	    		break;	    	
				
		if(i==3){
			for(int j=0; j<3; j++){
	        	out = wait(NULL);
	        	printf("p:%d-w:%d\n", getpid(), out);
	        }
	    }
	}
	else{ 
		sleep(1);
		showtree();
		out = wait(NULL);
	    printf("p:%d-w:%d\n", getpid(), out);
	    }
	    
	
	sleep(2);
	return EXIT_SUCCESS;
}