#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
//Crear un segmento de memoria compartida que guarde un double
int main(){
    double *p = NULL;
    int shmid = shmget((key_t)202223, sizeof(double), IPC_CREAT|0600); //0600 permiso de lectura y escritura para el propietario
    if(shmid != -1) printf("ok {%d}\n", shmid);
    else{perror("shmget"); exit(1);}
    p = (double *)shmat(shmid, NULL, 0);

    *p = 6.3; 

    shmdt(p);
    shmctl(shmid, IPC_CREAT, 0);

    return EXIT_SUCCESS;
}