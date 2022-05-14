#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
	int tub[2][2];

	pipe(tub[0]);pipe(tub[1]);
	if(!fork()){ //h0
		int data;		
		if(!fork()){//h1
			int data = 50;
			close(tub[0][0]);
			close(tub[0][1]);
			close(tub[1][0]);
			printf("nieto escrito %d \n", data);
			write(tub[1][1], &data, sizeof(int));					
		}else{//h0
			close(tub[0][0]);
			close(tub[1][1]);
			read(tub[1][0], &data, sizeof(int));
			write(tub[0][1], &data, sizeof(int));
			printf("leido %d \n", data);
			wait(NULL);		}
	}
	else{//padre
		int data;
		close(tub[1][0]);
		close(tub[1][1]);
		close(tub[0][1]);
		read(tub[0][0], &data, sizeof(int));
		printf("data=%d\n", data);
		wait(NULL);		
	}

	return EXIT_SUCCESS;
}