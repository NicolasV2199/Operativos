#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>


int main(){
	char *ptr;
	int shm_id, c=0;
	int shm_size = 256;

	shm_id=shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
	
	if(!fork()){				
		printf("[%d]child process started\n", getpid() );
		ptr = (char *) shmat(shm_id, 0, 0);
		char old[256];
		strcpy(old, ptr);
		do{
		if(strcmp(old, ptr)!=0){
			printf("<--%s\n", ptr);
			strcpy(old, ptr);}
		}while(strcmp(ptr,"quit") != 0 );
		
		printf("[%d]shm_value->%s\n", getpid(), ptr);				
		shmdt(ptr);
	}
	else{
		printf("[%d]parent process started\n", getpid() );
		ptr = (char *) shmat(shm_id, 0, 0);
		char msg[256];
		do{				
			fgets(msg, 256, stdin);
			msg[strlen(msg)-1] = '\0';
			strcpy(ptr, msg);
		}while(strcmp(ptr,"quit") != 0 );	
		
		wait(NULL);
		
		shmdt(ptr);
		shmctl(shm_id, IPC_RMID, 0);
	}
	return EXIT_SUCCESS;
}