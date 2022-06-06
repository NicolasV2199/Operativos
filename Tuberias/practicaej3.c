#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
//#include <sys/types.h>


int main(){ //Pasar un entero por tuberias
   int fd[2],n;   
   pipe(fd);

   if(fork()){ //padre
      int numero =5;
      close(fd[0]);      
      sleep(1);
      write(fd[1], &numero, sizeof(int));
      printf("[%d]Padre escrito:--> %d\n", getpid(), numero);
      close(fd[1]);
      wait(NULL);
   }
   else{//hijo
      int get;
      close(fd[1]);    
      printf("---->\n");  
      sleep(2);
      read(fd[0], &get, sizeof(int));
      printf("[%d]Hijo leido:<-- %d\n",getpid(), get );
      close(fd[0]);
   }
return EXIT_SUCCESS;
}
