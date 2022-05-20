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

//Funcion para hacer la multiplicacion de matrices
int get_mult_result(int **a, int **b, int n, int i, int j) {
	int res = 0;
	for (int k = 0; k < n; k++) {
		res += a[i][k] * b[k][j];
	}
	return res;
} 

int main(){

    oldhandler = signal( SIGUSR1, sighandler);     
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}

    
    int **resultado= NULL; //Declaramos un puntero a la matriz
    int **matriza= NULL; 
    int **matrizb= NULL; 

    int tamanio, i; 
    printf("Que tamaño de matriz desea? ");
    scanf("%d", &tamanio);
    

    size_t sizeMatrix = sizeof_dm(tamanio,tamanio,sizeof(int)); //Obtenemos el tamaño en memoria 
    int shmIdr = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600); //Se crea el segmento de memoria con permisos de lectura y escritura
    resultado = shmat(shmIdr, NULL, 0); // A la matriz le acoplamos el segmento de memoria compartida
    create_index((void*)resultado, tamanio, tamanio, sizeof(int)); //Definimos los indices de la matriz

    int shmIda = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600); //Se crea el segmento de memoria con permisos de lectura y escritura
    matriza = shmat(shmIda, NULL, 0); // A la matriz le acoplamos el segmento de memoria compartida
    create_index((void*)matriza, tamanio, tamanio, sizeof(int)); //Definimos los indices de la matriz

    int shmIdb = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600); //Se crea el segmento de memoria con permisos de lectura y escritura
    matrizb = shmat(shmIdb, NULL, 0); // A la matriz le acoplamos el segmento de memoria compartida
    create_index((void*)matrizb, tamanio, tamanio, sizeof(int)); //Definimos los indices de la matriz

    pid_t root = getpid();

    //Definimos las matrices
	srand (time(NULL));

	for(int p=0; p<tamanio; p++){
		for(int q=0; q<tamanio; q++){
			matriza[p][q]=rand()%10+1;
			matrizb[p][q]=rand()%10+1;
		}
	}

//Mostramos las matrices creadas
    printf("\nMatriz a\n");
		for(int c=0; c<tamanio; c++){
			printf("[ ");
			for (int l=0; l<tamanio; l++)
			{
				printf("%d ", matriza[c][l]);
			}
			printf(" ]\n");
		}
	printf("\nMatriz b\n");
		for(int c=0; c<tamanio; c++){
			printf("[ ");
			for (int l=0; l<tamanio; l++)
			{
				printf("%d, ", matrizb[c][l]);
			}
			printf(" ]\n");
		}
    printf("\n");

    // Creacion de los hijos
    int hijos;
    if(tamanio%2==0) hijos = tamanio/2;
	else hijos = (tamanio/2)+1;
    for (i = 0; i < hijos; i++) if (!fork()) break;

    if(i == hijos){ //Si es el padre
        for (int s = 0; s < hijos; ++s) wait(NULL);
        printf("\nMatriz resultado (padre): [%d]\n", getpid());
		for(int c=0; c<tamanio; c++){
			printf("[ ");
			for (int l=0; l<tamanio; l++)
			{
				printf("%d ", resultado[c][l]);
			}
			printf(" ]\n");
		}
    }else{ //Son los hijos
        // Estoy en el i-esimo hijo
		printf("[%d] - Hijo %d procesa [%d, %d]\n", getpid(), i, i, tamanio - 1 - i);
    
        for (int col = i; col < tamanio - i; col++) {
			resultado[i][col] = get_mult_result(matriza, matrizb, tamanio, i, col);
			resultado[tamanio - 1 - i][col] = get_mult_result(matriza, matrizb, tamanio, tamanio - 1 - i, col);
		}

		for (int row = i; row < tamanio - i; row++) {
			resultado[row][i] = get_mult_result(matriza, matrizb, tamanio, row, i);
			resultado[row][tamanio - 1 - i] = get_mult_result(matriza, matrizb, tamanio, row, tamanio - 1 - i);
		}
    }
   

    return EXIT_SUCCESS;
}

