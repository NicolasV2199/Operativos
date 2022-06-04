#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#define n_childs 3

void *oldhandle;
void sighandle(int sig) {
    // printf("[%d] signal %d cached\n", getpid(), sig);
}

int main() {

    pid_t pidchilds[n_childs];
    int i, h;

    //SET SIGNAL
    oldhandle = signal(SIGUSR1, sighandle);
    if (oldhandle == SIG_ERR) {
        perror("signal:");
        exit(EXIT_FAILURE);
    }

    //CREATION OF CHILDREN
    for (i=0 ; i < n_childs ; i++) {
        pidchilds[i] = fork();
        if (pidchilds[i] == 0) {
            if (i%2 != 0) {
                h = fork();
                break;
            }
            break;
        }
    }

    if ( i == 3) { // PADRE
        printf("Padre PID[%d]\n", getpid());
        usleep(10000);
        kill(pidchilds[2], SIGUSR1);
        pause();
        printf("Padri Pid [%d] Padre_end\n", getpid());
        for (int j = 0; j < n_childs; j++) wait(NULL);
    }
    else { //HIJOS
        pause();
        printf("Hijo PID[%d] | Padre PID[%d]\n", getpid(), getppid());
        if (i%2 != 0) {
            if ( h != 0) {
                kill(h, SIGUSR1);
                pause();
                printf("Hijo PID[%d] | Padre PID[%d]\n", getpid(), getppid());
                kill(pidchilds[i-1], SIGUSR1);
            }
            else {
                kill(getppid(), SIGUSR1);
            }
        }
        else {
            if (i == 0) {
                kill(getppid(), SIGUSR1);
            }
            else {
                kill(pidchilds[i-1], SIGUSR1);
            }
        }
    }

    //BACK TO ORIGIN SIGNAL
    if(signal(SIGUSR1, oldhandle) == SIG_ERR){
        perror("signal:");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}