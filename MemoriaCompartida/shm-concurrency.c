#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main(){
	int *ptr;
	int shm_id, i, j, c=0, max_iter=10000000;
	int shm_size = sizeof(int);

	shm_id=shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
	ptr = (int *) shmat(shm_id, 0, 0);
	*ptr = 0;

	if(!fork()){				
		printf("[%d]child process started\n", getpid() );		
		
		for (i = 0; i < max_iter; ++i)
		{
			*ptr = *ptr+1;
			//usleep(2);
		}
		//printf("[%d]--[%d] iterations *ptr->%d\n", getpid(),i,*ptr);				
		shmdt(ptr);
	}
	else{
		printf("[%d]parent process started\n", getpid() );
		
		for (j = 0; j < max_iter; ++j)
		{
			*ptr = *ptr+1;
			//usleep(1);
		}
						
		wait(NULL);		

		printf("[%d]--[%d] iterations *ptr->%d\n", getpid(), j,*ptr);				
		shmdt(ptr);
		shmctl(shm_id, IPC_RMID, 0);
	}
	return EXIT_SUCCESS;
}

