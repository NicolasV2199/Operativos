#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#define n_childs 4

void *oldhandle;
void sighandle(int sig) {
    // printf("[%d] signal %d cached\n", getpid(), sig);
}

int main() {
    /*Programa que crea 2 hijos, y cada hijo tiene un hijo.
    La raiz debe mandar una señar al hijo 2, y este mandar una señal a su hijo
    luego devolverla al padre, y este mandar una señal al hijo 1. 
    Este manda una señal a su hijo, luego la devuelve al padre, y finalmente a la raíz*/

    pid_t pidchilds[n_childs];
    int i;

    //SET SIGNAL
    oldhandle = signal(SIGUSR1, sighandle);
    if (oldhandle == SIG_ERR) {
        perror("signal:");
        exit(EXIT_FAILURE);
    }
    
    pid_t root = getpid();
    for(i=0 ; i < 2 ; i++) {
        pidchilds[i] = fork();
        if(!pidchilds[i]) break;
    }

    if(root == getpid()) { //LA RAIZ
        printf("PADRE: [%d]\n", getpid());        
        usleep(10000);
        kill(pidchilds[1], SIGUSR1);
        pause();
        printf("PADRE EF: [%d]\n", getpid());   

        for(int j=0; j<n_childs; j++)wait(NULL);
    }
    else { //HIJOS
        //Cada hijo crea sus hijos
        pid_t pidhijo = fork();
        pidchilds[i+2] = pidhijo;
        
        if (!pidhijo) { //HIJO del HIJO
            pause();
            // printf("HIJO DEL PROCESO [%d] => PID[%d]\n", getppid(), getpid());
            printf("HIJO [%d]\n", getpid());
            kill(getppid(), SIGUSR1);
        }
        else { //PADRE (Primeros hijos creados)
            pause();
            // printf("HIJO [%d] => [%d]\n", i+1, getpid());    
            printf("HIJO [%d]\n", getpid());    
            usleep(10000);
            kill(pidchilds[i+2], SIGUSR1);
            pause();
            // printf("HIJO [%d] DESPUES RS DE [%d]\n", i+1, pidchilds[i+2]);
            printf("HIJO [%d]\n", getpid());
            wait(NULL);
        }

        if(i!=0) //Estoy en el hijo de la derecha, que debe mandar una señal a su hermano de la izquierda
            kill(pidchilds[i-1], SIGUSR1);
        else
            kill(getppid(), SIGUSR1); //Estoy en el primer hijo, que debe devolver la señal al padre
    }

    //DEJA TODO COMO ESTABA (BUENA PRACTICA)
    if(signal(SIGUSR1, oldhandle) == SIG_ERR){
        perror("signal:");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}