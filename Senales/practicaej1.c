#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

void * oldhandler;

void sighandler( int sig ){
    //printf("senal recibida %d\n", sig);
}

int main(){
    oldhandler = signal( SIGUSR1, sighandler); 
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}
    
    int n;
    int i, hijos = 2;
    pid_t pidchilds[hijos];
    pid_t root = getpid();

    printf("Cuantas repeticiones quiere?");
    scanf("%d", &n);

    //Creacion de hijos
    for(i = 0; i<hijos; i++){
        pidchilds[i] = fork();
        if(!pidchilds[i]) break;
    }

    for(int j = 0; j<n; j++){
        if(root == getpid()){//Es el padre
            pause();
            printf("PADRE: [%d]\n", getpid());        
            usleep(10000);
            kill(pidchilds[1], SIGUSR1);

            for(int k=0; k<hijos; k++)wait(NULL);
        }else{ //Los hijos
            if(i == 0){
                usleep(1000000);
                kill(getppid(), SIGUSR1);
                printf("Hijo 1: [%d] Ping\n", getpid());
            }
            if(i==1){
                pause();
                printf("Hijo 2: [%d] Pong\n", getpid());
            }
        }
   }

    if(signal(SIGUSR1, oldhandler) == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}
    return EXIT_SUCCESS;
}