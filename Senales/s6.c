#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>


void * oldhandler;

void sighandler( int sig ){}

int main(){
    pid_t pidhijo[2], root;
    int i, j;
    
    oldhandler = signal( SIGUSR1, sighandler);     
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}
    
    root= getpid();
    for(i=0; i<2; i++){
        pidhijo[i] = fork();
        if(!pidhijo[i]) break;
    }    
    if(root == getpid()){ //Padre
        printf("Padre[%d]\n", getpid());
        usleep(100);
        kill(pidhijo[1], SIGUSR1);
        pause();
        printf("Padre[%d]\n", getpid());
        for( j=0; j<2; j++)wait(NULL);
    }
    else{ //Hijos
        pause();
        printf("hijo[%d]\n", getpid());
        if(i==1)
            kill(pidhijo[0], SIGUSR1);
        else
            kill(getppid(), SIGUSR1);

    }



    return EXIT_SUCCESS;
}