#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

void sig_handler(int s){};

int main(){
	pid_t child;
	int *ptr=NULL;
	int shm_id, c=0;
	int shm_size = sizeof(int)*10;

	shm_id=shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
	signal(SIGUSR1, sig_handler);

	child = fork();
	if(!child){				
		printf("[%d]child process started\n", getpid() );
		ptr = (int *) shmat(shm_id, 0, 0);
		pause();
		printf("[%d]reading shm_values:\n", getpid());				
		for(int i=0; i<10; i++)		
		  printf("%d ", ptr[i]);
		printf("\n");
		shmdt(ptr);
	}
	else{
		printf("[%d]parent process started\n", getpid() );
		int vect[10]={10,20,5,0,2,14,1,8,9,0};
		ptr = (int *) shmat(shm_id, 0, 0);

		usleep(3000);		
		for(int i=0; i<10; i++)		
		  ptr[i] = vect[i];				
		printf("[%d]written shm_values.\n", getpid());				
		kill(child, SIGUSR1);
		wait(NULL);
		
		shmdt(ptr);
		shmctl(shm_id, IPC_RMID, 0);
	}
	return EXIT_SUCCESS;
}