#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

int main(){

    int tub1[2],tub2[2], tub3[2], n;
    char buff[100];
    char mensaje[100];
    //Creacion de tuberia
    pipe(tub1);
    pipe(tub2);
    pipe(tub3);

	if(fork()){ //padre
      printf("Digite su mensaje: "); 
      scanf("%100[^\n]s",mensaje);

      close(tub1[0]);
      close(tub3[1]);

      sleep(1);
      write(tub1[1], mensaje, strlen(mensaje));
      printf("[%d] Padre escribe:--> [%s]\n", getpid(), mensaje);
      wait(NULL);

      n = read(tub3[0], buff, 100);
      buff[n] = '\0';
      printf("\n[%d]Padre muestra mensaje final:--> %s\n\n",getpid(), buff );
      close(tub1[1]);
      close(tub2[0]);
      close(tub2[1]);
      close(tub3[0]);

   }
   else{//hijo
        if(fork()){//padre
           
            //Seguimos con el proceso que viene de la raíz, cerrando escritura y leyendo
            close(tub1[1]);
            close(tub2[0]);
            close(tub3[0]);
            close(tub3[1]);

            n = read(tub1[0], buff, 100);
            buff[n] = '\0';
            printf("[%d]Hijo 1 lee \n",getpid());
            //invertimos el mensaje y lo mostramos en pantalla
            int longitud = strlen(buff);
            char temporal;
            for (int izquierda = 0, derecha = longitud - 1; izquierda < (longitud / 2);
                izquierda++, derecha--) {
                temporal = buff[izquierda];
                buff[izquierda] = buff[derecha];
                buff[derecha] = temporal;
            }
            sleep(1);
            write(tub2[1], buff, strlen(buff));
            printf("[%d] Hijo 1 escribe:--> [%s]\n", getpid(), buff);
            wait(NULL);
            close(tub1[0]);
            close(tub2[1]);
        }else{
            close(tub1[0]);
            close(tub1[1]);
            close(tub2[1]);
            close(tub3[0]);

            n = read(tub2[0], buff, 100);
            buff[n] = '\0';
            printf("[%d] Hijo del hijo lee\n",getpid() );
            for (int indice = 0; buff[indice] != '\0'; ++indice){
                buff[indice] = toupper(buff[indice]);
            }
           sleep(1);
            write(tub3[1], buff, strlen(buff));
            printf("[%d]Hijo del hijo escribe:--> [%s]\n", getpid(), buff);
            close(tub2[0]);
            close(tub3[1]);
        }
   }
	return EXIT_SUCCESS;
}