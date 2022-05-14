#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int main(){
   int fd[2], n;
   char Buffer_out[1024];
   char Buffer_in [1024];

   char data[] = {"Hola mundo pipes"};
   
   pipe(fd);
   
   strcpy(Buffer_out, data);
   printf("[%d]write:--> %s\n", getpid(), Buffer_out);
   write(fd[1], Buffer_out, strlen(Buffer_out)); //EOL = '\0'
	
   n = read(fd[0], Buffer_in, 1024);
   Buffer_in[n] = '\0';
   printf("[%d]read:<-- %s\n",getpid(), Buffer_in );      

return EXIT_SUCCESS;
}
