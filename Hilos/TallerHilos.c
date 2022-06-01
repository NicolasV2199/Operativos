#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_hilo(void*);

int valor=0, max_iter=10000000;

pthread_mutex_t mt = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
	int a;
	int b;
} Nodo;

int leerNumeros(char *filename, int **datos1, int **datos2){
	
	int n = 0;
	FILE *file = fopen(filename, "r");
	if(file==NULL){
        perror("archivos datos"); 
        exit(1);
    }
	fscanf(file, "%d", &n);
 	*datos1 = (int*)calloc((n),sizeof(int));
	*datos2 = (int*)calloc((n),sizeof(int));

 	if(*datos1==NULL || *datos2==NULL){
         perror("fallo memoria datos"); 
         exit(1);
    }
 	for (int i = 0; i < n*2; ++i){
		 if(i<n){
			fscanf(file, "%d", &((*datos1)[i]));
		 }else{
			 fscanf(file, "%d", &((*datos2)[i-n]));
		 }
 		
 	}
 	fclose(file);
 	return n;
}

void indices(int i, int hilos, int n, Nodo *nodo){
	int rango = n/hilos;
	int inicio = i*rango;
	int final = (inicio + rango) - 1;
	nodo->a = inicio;
	nodo->b = final;
	//printf("Rango hilo %d: %d %d\n", i,inicio,final);
}


int *vector1=NULL;
int *vector2=NULL;
int sumaFinal = 0;
int main(){
    
    int cantHilos, longVector; 
	Nodo datos;

	//Abro el archivo (debe estar en la misma carpeta) y lo divido en 2 vectores
	char*nombreArchivo = "input1.txt";
    longVector = leerNumeros(nombreArchivo, &vector1, &vector2);

	//Mostramos los vectores iniciales
	printf("Vector 1\n");
    for(int i=0; i<longVector; i++){
        printf("[%d]", vector1[i]);
    }
	printf("\nVector 2\n");
    for(int i=0; i<longVector; i++){
        printf("[%d]", vector2[i]);
    }

	printf("\n\nCuantos hilos (incluyendo al principal) desea: ");
    scanf("%d", &cantHilos);
	printf("\n\n");

	Nodo aux;
	pthread_t tid[cantHilos];
	if(cantHilos > 0){//Se crean los hilos nuevos
		for(int i=0; i<cantHilos; i++){
			aux = (Nodo)malloc(sizeof(Nodo));		
			indices(i, cantHilos, longVector, &aux);	
			pthread_create(&tid[i], NULL, funcion_hilo, (void*)&aux);	
		}
	}else{ //Se hace todo en el hilo principal
		for(int i = 0; i<longVector; i++){
			sumaFinal += vector1[i]*vector2[i];
		}
	}

	for(int i=0; i<cantHilos; i++)
		pthread_join(tid[i], NULL);
	
	printf("La suma final es: %d\n", sumaFinal);
	
	return EXIT_SUCCESS;
}



void* funcion_hilo(void* arg){	
	Nodo* p = (Nodo *)arg;
	printf("Hilo [%lu] hara inicio: [%d] fin: [%d]\n", pthread_self(), p->a, p->b);
	for(int i = p->a; i<=p->b; i++){
		sumaFinal += vector1[i]*vector2[i];
	}
	pthread_exit(0);
}