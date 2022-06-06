#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#define READ      0
#define WRITE     1

int main(){

    char buff[100];
    char mensaje[100];
    int i, n=5;
    int tub[n][2];
    pid_t pidchilds[n];
    
    printf("Digite una palabra de 5 letras: "); 
    scanf("%100[^\n]s",mensaje);
    printf("\n");

    for(int j = 0; j<n+1; j++){
        pipe(tub[j]);
    }

    for( i =0; i<n; i++){//Creo los hijos
        pidchilds[i] = fork();
        if(!pidchilds[i]) break;
    }

    if(i==n){//padre
    //Primero es el numero de la tuberia, y luego el extremo que quiere cerrar
        close(tub[0][WRITE]);
        close(tub[1][READ]); 
        for(int j= 2; j<=n; j++){
            close(tub[j][READ]);
            close(tub[j][WRITE]);
        }
        sleep(1);
        write(tub[1][1], mensaje, strlen(mensaje));
        printf("[%d] Padre escribe:--> [%s]\n", getpid(), mensaje);
        //pause();
        for(int j = 0; j<n; j++) wait(NULL);
        n = read(tub[0][0], buff, 100);
        buff[n] = '\0';
        printf("\n[%d]Padre Final lee:--> [%s]\n\n",getpid(), buff );
        close(tub[0][READ]);
        close(tub[1][WRITE]); 
    }else{//HIJOS
        switch(i){
            
            case (0): {
                close(tub[0][READ]);
                close(tub[5][WRITE]);
                for(int j= 1; j<n; j++){
                    close(tub[j][WRITE]);
                    close(tub[j][READ]);
                }
                n = read(tub[5][0], buff, 100);
                buff[n] = '\0';
                printf("\n[%d]Hijo [%d] lee:--> [%s]\n",getpid(), i, buff );

                sleep(1);
                buff[i] = toupper(buff[i]);
                write(tub[0][1], buff, strlen(buff));
                printf("[%d] Hijo [%d] escribe:--> [%s]\n", getpid(), i, buff);
                close(tub[0][WRITE]);
                close(tub[5][READ]);
                break;
            }
            case (1): {
                close(tub[5][READ]);
                close(tub[4][WRITE]);
                for(int j= 0; j<n-1; j++){
                    close(tub[j][WRITE]);
                    close(tub[j][READ]);
                }

                n = read(tub[4][0], buff, 100);
                buff[n] = '\0';
                printf("\n[%d]Hijo [%d] lee:--> [%s]\n",getpid(), i, buff );

                sleep(1);
                buff[i] = toupper(buff[i]);
                write(tub[5][1], buff, strlen(buff));
                printf("[%d] Hijo [%d] escribe:--> [%s]\n", getpid(), i, buff);
                close(tub[5][WRITE]);
                close(tub[4][READ]);
                break;
            } 
            case (2): {
                close(tub[4][READ]);
                close(tub[3][WRITE]);

                close(tub[0][WRITE]);
                close(tub[0][READ]);
                close(tub[1][WRITE]);
                close(tub[1][READ]);
                close(tub[2][WRITE]);
                close(tub[2][READ]);
                close(tub[5][WRITE]);
                close(tub[5][READ]);
                
                n = read(tub[3][0], buff, 100);
                buff[n] = '\0';
                printf("\n[%d]Hijo [%d] lee:--> [%s]\n",getpid(), i, buff );

                sleep(1);
                buff[i] = toupper(buff[i]);
                write(tub[4][1], buff, strlen(buff));
                printf("[%d] Hijo [%d] escribe:--> [%s]\n", getpid(), i, buff);
                close(tub[4][WRITE]);
                close(tub[3][READ]);
                break;
            }
            case (3): {
                close(tub[3][READ]);
                close(tub[2][WRITE]);
                
                close(tub[0][WRITE]);
                close(tub[0][READ]);
                close(tub[1][WRITE]);
                close(tub[1][READ]);
                close(tub[4][WRITE]);
                close(tub[4][READ]);
                close(tub[5][WRITE]);
                close(tub[5][READ]);

                n = read(tub[2][0], buff, 100);
                buff[n] = '\0';
                printf("\n[%d]Hijo [%d] lee:--> [%s]\n",getpid(), i, buff );

                sleep(1);
                buff[i] = toupper(buff[i]);
                write(tub[3][1], buff, strlen(buff));
                printf("[%d] Hijo [%d] escribe:--> [%s]\n", getpid(), i, buff);
                close(tub[3][WRITE]);
                close(tub[2][READ]);
                break;
            }
            case (4): {
                close(tub[2][READ]);
                close(tub[1][WRITE]);
                
                close(tub[0][WRITE]);
                close(tub[0][READ]);
                close(tub[3][WRITE]);
                close(tub[3][READ]);
                close(tub[4][WRITE]);
                close(tub[4][READ]);
                close(tub[5][WRITE]);
                close(tub[5][READ]);

                n = read(tub[1][0], buff, 100);
                buff[n] = '\0';
                printf("\n[%d]Hijo [%d] lee:--> [%s]\n",getpid(), i, buff );

                sleep(1);
                buff[i] = toupper(buff[i]);
                write(tub[2][1], buff, strlen(buff));
                printf("[%d] Hijo [%d] escribe:--> [%s]\n", getpid(), i, buff);
                close(tub[2][WRITE]);
                close(tub[1][READ]);
                break;
            }
            default: printf("ERROR EN SWITCH");break;
        }
    }


	return EXIT_SUCCESS;
}