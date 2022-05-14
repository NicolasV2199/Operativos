#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int count = 0;
void * oldhandler;

void sighandler( int sig ){
    count ++;
}

int main(){
    oldhandler = signal( SIGINT, sighandler); 
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}
    printf("[%d]\n", getpid());
    do{
        printf("%d veces SIGINT recibida\n", count);
        sleep(1);
    }while(1);
    

    if(signal(SIGUSR1, oldhandler) == SIG_ERR){perror("signal:");exit(EXIT_FAILURE); }
    return EXIT_SUCCESS;
}