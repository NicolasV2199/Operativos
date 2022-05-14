#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
//#include <sys/types.h>


int main(){
   int fd[2],n;
   char buff[50];
   
   pipe(fd);
   
   if(fork()){ //padre
      char msg[] = {"Mensaje de texto"};
      
      close(fd[0]);      
      sleep(5);
      write(fd[1], msg, strlen(msg));
      printf("[%d]escrito:--> %s\n", getpid(), msg);
      close(fd[1]);
      wait(NULL);
   }
   else{//hijo
      close(fd[1]);    
      printf("---->\n");  
      n = read(fd[0], buff, 50);
      buff[n] = '\0';
      printf("[%d] leido:<-- %s\n",getpid(), buff );
      close(fd[0]);
   }
return EXIT_SUCCESS;
}
