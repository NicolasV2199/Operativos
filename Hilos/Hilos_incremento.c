#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_hilo(void*);

int valor=0, max_iter=10000000;

pthread_mutex_t mt = PTHREAD_MUTEX_INITIALIZER;

int main(){
	int maxhilos = 7; 
	pthread_t tid[maxhilos];
	for(int i=0; i<maxhilos; i++){		
		pthread_create(&tid[i], NULL, funcion_hilo, NULL);		
	}
	printf("Hilo principal[%lu]\n", pthread_self()); 

	for(int i=0; i<maxhilos; i++)
		pthread_join(tid[i], NULL);
	
	printf("valor=[%d]\n", valor);
	
	
	return EXIT_SUCCESS;
}


void* funcion_hilo(void* arg){	
	printf("Hilo [%lu] hará [%d] iteraciones\n", pthread_self(), max_iter);

	for (int i = 0; i < max_iter; ++i){
		pthread_mutex_lock(&mt);
		valor = valor+1;	
		pthread_mutex_unlock(&mt);

	}	
	pthread_exit(0);
}