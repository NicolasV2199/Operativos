#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_hilo(void*);

struct nodo{
	int a;	
};

int numero=0;

int main(){
	int maxhilos = 4;
	pthread_t tid[maxhilos];
	struct nodo dato;	
	
	for(int i=0; i<maxhilos; i++){
		dato.a = i;
		pthread_create(&tid[i], NULL, funcion_hilo, (void*)&dato);
	}
	printf("Hilo principal[%lu]\n", pthread_self());
	for(int i=0; i<maxhilos; i++){
		pthread_join(tid[i], NULL);
	}
	printf("%d\n", numero);
	return EXIT_SUCCESS;
}

void* funcion_hilo(void* arg){
	struct nodo* p = (struct nodo *)arg;
	printf("Hilo [%lu] dato[%d]\n", pthread_self(), p->a);
	numero = 500;
	pthread_exit(0);
}
