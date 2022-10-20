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
    int cantSensores;
    int i, n=5;
    
    
    printf("Digite la cantidad de sensores que desea: "); 
    scanf("%d",cantSensores);
    printf("\n");

    int tub[cantSensores][2];
    pid_t pidchilds[cantSensores];

    for(int j = 0; j<cantSensores; j++){//Creo la tuberia
        pipe(tub[j]);
    }

    for( i =0; i<cantSensores; i++){//Creo los hijos
        pidchilds[i] = fork();
        if(!pidchilds[i]) break;
    }

    //Funcion de buscar numero de la tuberia
    buscarTuberiaHijo(idsensor){
        for(int j= 0; j< cantSensores; i++){
            if(pidchilds[i] == idsensor){
                return i;
            }
        }
    }

    if(i==cantSensores){//padre
        int idsensor;
        int tiempo;
        char orden[100];
        int k;

        //Recorrer el arreglo de pids hijos, y mostrarlos

        //Primero es el numero de la tuberia, y luego el extremo que quiere cerrar
            for(int j = 0; j < cantSensores; j++){
                close(tub[j][READ]);
            }
            do{
                printf("Datos de la orden:");
                printf("Digite el id del sensor: ");
                scanf("%d",idsensor);
                printf("Digite el tiempo: ");
                scanf("%d",tiempo);
                printf("Digite su orden: ");
                scanf("%s",orden);
                //Conseguir el idSensor (que coincide con el pid del hijo que va a encargarse de ese sensor)
                k = buscarTuberiaHijo(idsensor); //Buscar el numero de tuberia a partir del idsensor
                write(tub[k][WRITE], order, strlen(orden)); //Escribir en la tuberia correspondiente la orden
                
                
                //Si vas a mandar la orden completa, seria asi: 
                printf("Digite la orden: "); // 5 5000 activar
                scanf("%s",orden);
                k = buscarTuberiaHijo(idsensor); 
                write(tub[k][WRITE], order, strlen(orden)); 
                //Divides la orden en el hijo, separando cada variable cuando encuentres un espacio
                sleep(1);
            }while(orden != "salir");

            printf("HA terminado el proceso servidor");
            for(int j = 0; j < cantSensores; j++){
                close(tub[j][WRITE]);
            }

    }else{//HIJOS

        //Cerrrar el extremo de escritura para los hijos
        for(int k = 0; k < cantSensores; k++){
            close(tub[k][WRITE]);
        }
        //Leyendo la tuberia y guardando la orden
        n = read(tub[i][READ], buff, 100); //5 5000 activar
        buff[n] = '\0'; 
        idsensor
        tiempo
        orden
        //Espacio para la funcion de separacion de la orden en 3 variables; 

        //impresion de que recibio la orden
        printf("pid Hijo " getpid(), "idSensor:" idsensor, "Recibio la orden"); 

        //Una vez separado el string, seguimos las instrucciones
        delay(tiempo);
        printf("hijo" idsensor, orden, "despues de: "tiempo);

        //Lo que vaya despues del sleep es porque ya termino (suponemos)
        sleep(); 
        for(int k = 0; k < cantSensores; k++){
            close(tub[k][READ]);
        }
    }


	return EXIT_SUCCESS;
}