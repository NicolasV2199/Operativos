#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>


int main(){

	int shmid, max_iter = 10000000;
	int * pointer=NULL;

	shmid   = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT|S_IRUSR|S_IWUSR);
	pointer = (int *)shmat(shmid, 0, 0);
	pointer[0] = pointer[1] = 0;

	if(!fork()){		
		for (int i = 0; i < max_iter; ++i){
			pointer[0] = pointer[0] + 1;
		}
		shmdt(pointer);
	}
	else{

		for (int i = 0; i < max_iter; ++i){
			pointer[1] = pointer[1] + 1;
		}
		wait(NULL);
		printf("the value of shm = [%d] \n", pointer[0]+pointer[1]);

		shmdt(pointer);
		shmctl(shmid, IPC_RMID, 0);
	}

	return EXIT_SUCCESS;
}