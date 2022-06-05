#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

int main(){
    int tub1[2], tub3[2], n;
    char buff[100];
    char mensaje[100];
    //Creacion de tuberia
    pipe(tub1);
    pipe(tub3);

	if(fork()){ //padre
      printf("Digite su mensaje: "); 
      scanf("%100[^\n]s",mensaje);

      close(tub1[0]);
      close(tub3[1]);
      write(tub1[1], mensaje, strlen(mensaje));
      printf("[%d]escrito:--> %s\n", getpid(), mensaje);
      wait(NULL);

   }
   else{//hijo
        if(fork()){//padre
        //Creamos la nueva tuberia y cerramos la lectura
            int tub2[2], p;
            char buff2[100];
            //Creacion de tuberia
            pipe(tub2);            
            //close(tub2[0]);

            //Seguimos con el proceso que viene de la raíz, cerrando escritura y leyendo
            close(tub3[0]);
            close(tub3[1]);
            close(tub1[1]);

            p = read(tub1[0], buff, 100);
            buff[p] = '\0';
            printf("[%d] leido:<-- %s\n",getpid(), buff );
            //invertimos el mensaje y lo mostramos en pantalla
            int longitud = strlen(buff);
            char temporal;
            for (int izquierda = 0, derecha = longitud - 1; izquierda < (longitud / 2);
                izquierda++, derecha--) {
                temporal = buff[izquierda];
                buff[izquierda] = buff[derecha];
                buff[derecha] = temporal;
            }
            printf("[%d] Cadena invertida:<-- %s\n",getpid(), buff );
           
        }else{
            
           
        }
   }
	return EXIT_SUCCESS;
}