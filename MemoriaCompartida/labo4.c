#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <math.h>

void sig_handler(int s){};
unsigned int sizeof_dm(int, int, size_t);
void create_index(void **, int, int, size_t);

int main(int argc, char const *argv[])
{
	int i, shmId, N=4, **matrix;
	srand (time(NULL));

	size_t sizeMatrix = sizeof_dm(N, N, sizeof(int));
	shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
	matrix = shmat(shmId, NULL, 0);
	create_index((void*) matrix, N, N, sizeof(int));
	
	pid_t root=getpid();
	
	//Para cambiar el tamaño de las matrices solo debe cambiar el valor de N...
	int a[N][N] , b[N][N];
	
	for(int p=0; p<N; p++){
		for(int q=0; q<N; q++){
			a[p][q]=rand()%10+1;
			b[p][q]=rand()%10+1;
			}
	}
	
	printf("\nMatriz a\n");
		for(int s=0; s<N; ++s){
			printf("[ ");
			for (int l=0; l<N; ++l)
			{
				printf("%d ", a[s][l]);
			}
			printf(" ]\n");
		}
	printf("\nMatriz b\n");
		for(int s=0; s<N; ++s){
			printf("[ ");
			for (int l=0; l<N; ++l)
			{
				printf("%d, ", b[s][l]);
			}
			printf(" ]\n");
		}


	signal(SIGUSR1, sig_handler);

	if(N%2==0) {
		for (i=0; i < N/2; ++i) if(!fork()) break;
		}
	else {
		for (i=0; i < (N/2)+1; ++i) if(!fork()) break;
		}
		

	if(root==getpid()){

		for(int s=0; s<N/2; ++s) pause();

		printf("\npadre imprime matriz c: [%d]\n", getpid());
		for(int s=0; s<N; ++s){
			printf("[ ");
			for (int l=0; l<N; ++l)
			{
				printf("%d ", matrix[s][l]);
			}
			printf(" ]\n");
		}

		usleep(8000);
		for(int s=0; s<N/2; ++s) wait(NULL);
		
		shmdt(matrix);
		shmctl(shmId, IPC_RMID, 0);

	}
	else{
		//parte de arriba del anillo 
		for(int j=i; j<N-i; j++){
            for(int m = i; m<N-i; m++){
                matrix[i][j] = 0;
            }
			for(int k=0; k<N-i; k++){
				matrix[i][j] += a[i][k]*b[k][j];
			}
			
		}
		
		//parte de la derecha del anillo arriba hacia abajo
		for(int j=i+1; j<N-i; j++){ //i= 0, j = 1; N = 3; m = 1; k=0 
            for(int m = i+1; m < N-i; m++){
                matrix[j][N-i-1] = 0;
            }
			for(int k=0; k<N-i; k++){
				matrix[j][N-i-1] += a[j][k]*b[k][N-i-1];
				}
			
		}
		
		//parte de abajo del anillo <-
		for(int j=N-1; j>=i; j--){
            for(int m = N-1; m >= i; m--){
                matrix[N-i-1][j] = 0;
            }
			for(int k=0; k<N-i; k++){
				matrix[N-i-1][j] += a[N-i-1][k]*b[k][j];
				}
			
		}
		
		//parte de la izquierda abajo hacia arriba del anillo
		for(int j=N-i-1; j>i; j--){
            for(int m = N-i-1; m>i; m--){
                matrix[j][i] = 0;
            }
			for(int k=0; k<N-i; k++){
				matrix[j][i] += a[j][k]*b[k][i];
				}
			
		}

		kill(root, SIGUSR1);
		shmdt(matrix);
	}

	return EXIT_SUCCESS;
}

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
	size_t size = rows * sizeof(void *); //indexSize
	size+= (cols * rows * sizeElement); //Data size
	return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
	int i;
	size_t sizeRow = cols * sizeElement;
	m[0] = m + (rows * sizeof(void *));
	for(i=1; i<rows; i++){
		m[i] = (m[i-1] + sizeRow);
	}
}
