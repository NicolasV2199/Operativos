#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

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

    printf("Que tamaño de matriz desea?");
    scanf()
    int Rows=3, Cols = 3; //Definimos el tamaño

    size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(double)); //Obtenemos el tamaño en memoria 
    int shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600); //Se crea el segmento de memoria con permisos de lectura y escritura
    matrix = shmat(shmId, NULL, 0); // A la matriz le acoplamos el segmento de memoria compartida
    create_index((void*)matrix, Rows, Cols, sizeof(double)); //Definimos los indices de la matriz

    pid_t hijo = fork();

    if(!hijo){ //Proceso hijo
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
    }

    return EXIT_SUCCESS;
}


