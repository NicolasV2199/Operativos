#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

//Mandar un entero por memoria compartida
int main(){
	int *ptr;
	int shm_id, c=0;
    int n = 5;
	shm_id=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
    ptr = (int *) shmat(shm_id, 0, 0);
	
	if(!fork()){				
		printf("[%d]child process started\n", getpid() );
		printf("[%d]shm_value->%d\n", getpid(), *ptr);				
		shmdt(ptr);
	}
	else{
		printf("[%d]parent process started\n", getpid() );
		//ptr = (int *) shmat(shm_id, 0, 0);	
		*ptr = n;
		wait(NULL);
		
		shmdt(ptr);
		shmctl(shm_id, IPC_RMID, 0);
	}
	return EXIT_SUCCESS;
}