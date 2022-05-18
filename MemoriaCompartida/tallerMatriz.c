#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <math.h>

void * oldhandler;

void sighandler( int sig ){
   // printf("senal recibida %d\n", sig);
}

//Funcion para definir el tamaño de la matriz
unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size;
    size= rows * sizeof(void *); //indexSize
    size+= (cols * rows * sizeElement);  //Data sizereturn size;
    return size;
}

//Funcion para establecer los indices de la matriz
void create_index(void **m, int rows, int cols, size_t sizeElement){
    int i;
    size_t sizeRow = cols * sizeElement; // El tamaño de una fila, sera el tamaño de una columna por el tamaño del elemento
    m[0] = m+rows;
    for(i=1; i<rows; i++){
        m[i] = (m[i-1]+sizeRow);
    }
}


int main(){

    oldhandler = signal( SIGUSR1, sighandler);     
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}

    
    double **matrix= NULL; //Declaramos un puntero a la matriz

    int tamanio, i; 
    printf("Que tamaño de matriz desea? ");
    scanf("%d", &tamanio);
    

    size_t sizeMatrix = sizeof_dm(tamanio,tamanio,sizeof(double)); //Obtenemos el tamaño en memoria 
    int shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600); //Se crea el segmento de memoria con permisos de lectura y escritura
    matrix = shmat(shmId, NULL, 0); // A la matriz le acoplamos el segmento de memoria compartida
    create_index((void*)matrix, tamanio, tamanio, sizeof(double)); //Definimos los indices de la matriz

    pid_t root = getpid();

    //Definimos las matrices
    int a[tamanio][tamanio] , b[tamanio][tamanio];
	srand (time(NULL));

	for(int p=0; p<tamanio; p++){
		for(int q=0; q<tamanio; q++){
			a[p][q]=rand()%10+1;
			b[p][q]=rand()%10+1;
		}
	}

//Mostramos las matrices creadas
    printf("\nMatriz a\n");
		for(int c=0; c<tamanio; c++){
			printf("[ ");
			for (int l=0; l<tamanio; l++)
			{
				printf("%d ", a[c][l]);
			}
			printf(" ]\n");
		}
	printf("\nMatriz b\n");
		for(int c=0; c<tamanio; c++){
			printf("[ ");
			for (int l=0; l<tamanio; l++)
			{
				printf("%d, ", b[c][l]);
			}
			printf(" ]\n");
		}

    // Creacion de los hijos

    if(tamanio%2==0) { //Si es tamaño par
		for (i=0; i < N/2; i++){
            if(!fork()) break;
        } 
	}
	else { //Si es tamaño impar
		for (i=0; i < (N/2)+1; i++){
            if(!fork()) break;
        } 
	}

    if(root = getpid()){ //Si es el padre
        printf("\nMatriz resultado (padre): [%d]\n", getpid());
		for(int c=0; c<tamanio; c++){
			printf("[ ");
			for (int l=0; l<N; l++)
			{
				printf("%d ", matrix[c][l]);
			}
			printf(" ]\n");
		}
    }

    /* if(!hijo){ //Proceso hijo
        pause();
        printf("hijo[%d]\n", getpid());
        for(int i = 0; i< Rows; i++){
            for(int j = 0; j< Cols; j++){
                printf("[%f] ", matrix[i][j]);
            }
            printf("\n");
        }
    }else{ //Proceso padre
        printf("Padre[%d]\n", getpid());
        usleep(100);
        kill(hijo, SIGUSR1);
        for(int i = 0; i< Rows; i++){
            for(int j = 0; j< Cols; j++){
                matrix[i][j] = 1; 
            }
        }
        wait(NULL);
    } */

    return EXIT_SUCCESS;
}


