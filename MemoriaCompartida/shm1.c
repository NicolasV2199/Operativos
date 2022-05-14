
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>


int main(){
	int *ptr=NULL;
	int shm_id, c=0;
	int shm_size = sizeof(int);
                   
	shm_id=shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
			
	if(fork() ==0 ){						
		ptr = (int *) shmat(shm_id, 0, 0);
		printf("[%d]child process started\n", getpid() );		
		ptr = (int *) shmat(shm_id, 0, 0);		
		do{
			usleep(2000);
			printf(".");
			fflush(stdout);
		}while(*ptr != 1); 
		printf("[%d]shm_value->%d\n", getpid(),*ptr);				
		
		shmdt(ptr);
	}
	else{
		ptr = (int *) shmat(shm_id, 0, 0);		
		//use
		*ptr=0;
		printf("[%d]parent process started\n", getpid() );
		sleep(2);
		*ptr = 1; //write into shm
		printf("[%d]shm_value->%d\n", getpid(),*ptr);				
		wait(NULL);

		
		shmdt(ptr);
		shmctl(shm_id, IPC_RMID, NULL);
	}
	return EXIT_SUCCESS;
}

