#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void * oldhandler;

void sighandler( int sig ){
    printf("senal recibida %d\n", sig);
}

int main(){
    oldhandler = signal( SIGUSR1, sighandler); 
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}
    /*
    .
    .
    .código
    .
    .
    */
    if(signal(SIGUSR1, oldhandler) == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}
    return EXIT_SUCCESS;
}