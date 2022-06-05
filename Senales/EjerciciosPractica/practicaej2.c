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

  //  for(int j=0; j<n; j++){

        if(root == getpid()){//Es el padre
            for(int j=0; j<n; j++){
            printf("PADRE: [%d]\n", getpid());        
            usleep(100000);
            kill(pidchilds[1], SIGUSR1);
            pause();
          }
            printf("Termine aqui, padre\n");
        
            for(int k=0; k<hijos; k++)wait(NULL);
        }else{ //Los hijos
            for(int j=0; j<n; j++){
                if(i == 0){
                    pause();
                    printf("Hijo 1: [%d] Pong\n", getpid());
                    kill(getppid(), SIGUSR1);
                }
                if(i==1){
                    pause();
                    printf("Hijo 2: [%d] Ping\n", getpid());
                    kill(pidchilds[0], SIGUSR1);
                }
            }
        }
   // }

    if(signal(SIGUSR1, oldhandler) == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}
    return EXIT_SUCCESS;
}