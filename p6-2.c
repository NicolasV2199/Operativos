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
	
	pid_t root = getpid(), out; 
	
	for(int i=0; i<3; i++){
    	if( !fork() ){
    		if( i==0 || i==2){
    			fork();
    		}
    	    break;
    	}
	}
	if( root == getpid() ){ 
		sleep(1);
		showtree();
	    for(int j=0; j<3; j++){
	        out = wait(NULL);
	        printf("p:%d-w:%d\n", getpid(), out);
	    }
	    
	}
	else{
		out = wait(NULL);
		if(out != -1)
	    	printf("p:%d-w:%d\n", getpid(), out);
	sleep(2);
	}
	
	return EXIT_SUCCESS;
}