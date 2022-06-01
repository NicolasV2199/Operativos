#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_hilo(void*);

struct nodo{
	int a;
	float b;
};

int main(){
	pthread_t tid;
	struct nodo dato;

	dato.a = 10;
	dato.b = 25.5;
	pthread_create(&tid, NULL, funcion_hilo, (void*)&dato);
	printf("Hilo principal[%lu]\n", pthread_self());
	pthread_join(tid, NULL);
	return EXIT_SUCCESS;
}

void* funcion_hilo(void* arg){
	struct nodo* p = (struct nodo *)arg;
	printf("Hilo [%lu] dato[%d | %f]\n", pthread_self(), p->a, p->b);
	pthread_exit(0);
}
