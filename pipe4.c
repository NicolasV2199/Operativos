#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_READ 256
#define EOL '\0'
char buff[MAX_READ];


int main(){
   int fd[2], n;
   pipe(fd);

   printf("write quit to exit\n\n");
   
   if(fork()!=0){
      close(fd[0]);
      do{        
         fgets(buff, MAX_READ, stdin );         
         if(strlen(buff)>1){
            buff[strlen(buff)-1] = '\0';
            printf("[%d]write-->:%s\n",getpid(),buff);
            write(fd[1], buff, strlen(buff));
         }
       }while(strcmp(buff,"salir") !=0);
      close(fd[1]);
      wait(NULL);		
    }
      
   else{
         //close(fd[1]);         
         while( (n=read(fd[0],buff, MAX_READ)) >0 ){
            buff[n] = '\0';
            printf("[%d]read<--:%s\n",getpid(),buff);                       
          }
      }
return EXIT_SUCCESS;
  }
