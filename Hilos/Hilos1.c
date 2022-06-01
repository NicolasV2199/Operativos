#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_hilo1(void*);
void* funcion_hilo2(void*);

pthread_t tid1;
int main(){
	//pthread_t tid;	
	pthread_create(&tid1, NULL, funcion_hilo1, NULL);	//fork()	
	printf("Hilo principal[%lu]\n", pthread_self());
	//pthread_join(tid1, NULL);  //wait()
	return EXIT_SUCCESS;
}
void* funcion_hilo1(void* arg){
	printf("Hilo1 [%lu]\n", pthread_self());
	pthread_exit(0);
}

