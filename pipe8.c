#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
	int tub[2][2], i; //tub1[2]; tub2[2];
	pid_t root = getpid();
	for (i = 0; i < 2; ++i) pipe(tub[i]);
	for (i = 0; i < 2; ++i) if(!fork()) break;

	if(root == getpid()){//padre		
		float datos[2];
		for (i = 0; i < 2; ++i) close(tub[i][1]);		
		
		for (int j = 0; j < 2; ++j)
			read(tub[j][0], &datos[j], sizeof(float));
		wait(NULL);wait(NULL);
		printf("suma total=%f\n", datos[0]+datos[1]);
		
	}
	else{		
		float dato;
		close(tub[0][0]);
		close(tub[1][0]);		
		close(tub[i==0?1:0][1]);

		if(i==0) dato = 25.32; //f(datos_del_padre)
		else     dato = 14.1;  //f(datos_del_padre)
		
		write(tub[i][1], &dato, sizeof(float));
		printf("[%d] -> %f\n", getpid(), dato);

	}
return EXIT_SUCCESS;
}