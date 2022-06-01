#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_hilo(void*);

int main(){
	pthread_t tid;
	int *dato;
	
	dato = 10;


	pthread_create(&tid, NULL, funcion_hilo, (void*) dato);
	printf("Hilo principal[%lu]\n", pthread_self());
	pthread_join(tid, NULL);
	return EXIT_SUCCESS;
}

void* funcion_hilo(void* arg){
	int* parametro = (int *)arg;
	printf("Hilo [%lu] dato[%d]\n", pthread_self(), *parametro);
	pthread_exit(0);
}
