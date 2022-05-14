#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>


void * oldhandler;
void sighandler( int sig ){
    //printf("[%d]sig %d capturada\n", getpid(), sig);    
}

#define n_hijos 3

int main(){
    pid_t pidhijo[n_hijos];
    int i,j, n, k;

    printf("N:?");
    scanf("%d", &n);

    oldhandler = signal( SIGUSR1, sighandler); 
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);  }

    int root = getpid();
    for(i=0; i<n_hijos; i++){
        pidhijo[i] = fork();
        if(!pidhijo[i]) break;
    }

    if(root == getpid()){

        for(k=0; k<n; k++){
            usleep(10000);
            printf("Padre [%d]\n", getpid());            
            kill(pidhijo[n_hijos-1], SIGUSR1);
            pause();
            printf("Padre [%d]\n", getpid());
        }
        for(j=0; j<n_hijos; j++)wait(NULL);
    }
    else{
        for(k=0; k<n; k++){
            pause();
            printf("hijo[%d]\n", getpid());
            if(i!=0)
                kill(pidhijo[i-1], SIGUSR1);
            else
                kill(getppid(), SIGUSR1);
        }
    }

    signal( SIGUSR1, oldhandler); 
    
    return EXIT_SUCCESS;
}