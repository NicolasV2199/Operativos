#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFF 1024

struct data{
   int a;
   float b;    //sizeof(struct) > (sizeof(int) + sizeof(float))
};

int main(){
   int fd[2], n;
   pipe(fd);

   if(fork()==0){
      struct data get;
      close(fd[1]);      

      n = read(fd[0], &get, sizeof(struct data));
      
      printf("[%d]read:<--\t[%d|%.2f]\n", getpid(), get.a, get.b);       
   }
   else{
      struct data *post=malloc(sizeof(struct data));
      close(fd[0]);
      post->a = 10;  
      post->b = 2.3;
      printf("[%d]write:-->\t[%d|%.2f]\n", getpid(), post->a, post->b); 
      
      write(fd[1], post, sizeof(struct data));
      free(post);
      
      wait(NULL);
   }
return EXIT_SUCCESS;	
}
