#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main( void ) {

	pid_t pidchild = fork();
	
    switch (pidchild)  
	{ 
        case -1:/* Error en la ejecución de fork(), se debe implementar la \\
                    función manejadora del error*/
                perror("Error fork"); 
            	exit(EXIT_FAILURE);
        case 0: /* Bloque proceso hijo */ 
                printf( "Proceso hijo\n" ); 
		        break;
        default: /* Bloque proceso padre */ 
                printf( "Proceso padre\n" );                
 	}
	return 0;
}