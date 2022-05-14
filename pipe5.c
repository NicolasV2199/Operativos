#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
	int tubs[2][2], i;
	int root = getpid();
	for (i = 0; i < 2; ++i)pipe(tubs[i]);
	
	for (i = 0; i < 2; ++i){
		if(!fork())break;
	}

	if(root == getpid()){
		float datos[2];
		close(tubs[0][1]);
		close(tubs[1][1]);
		for (i = 0; i < 2; ++i){
			read(tubs[i][0], &datos[i], sizeof(float));
			printf("hijo[%d]<-%f\n", i, datos[i]);
		}
		printf("suma total = %f\n", datos[0]+datos[1]);
		wait(NULL);wait(NULL);
	}
	else{
		int tc = i==1?0:1;
		float data;
		close(tubs[0][0]);
		close(tubs[1][0]);
		close(tubs[tc][1]);
		if(i==0){ data=10.3;}
		else    { data=8.4;}
		write(tubs[i][1], &data, sizeof(data));
	}

	return EXIT_SUCCESS;	
}