#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

int get_mult_result(int**, int**, int, int, int);
int** create_shared_matrix(int, int, int*);
int** create_and_fill_shared_matrix(int, int, int*);
unsigned int sizeof_shmt(int, int, size_t);
void create_index_segment(void**, int, int, size_t);
void fill_matrix(int, int, int**);
void throw_error(char*);

int main (int argc, char **argv) {
	
	int n = 0;
	printf("Digite el tamanio de la matriz de n x n: ");
	scanf("%d", &n);
	if (n < 2) throw_error("La matriz debe ser almenos de 2x2");

	const int rows = n, cols = n;
	int i = 0; int shm_ids[] = {0, 0, 0};
	int **matrixa = create_and_fill_shared_matrix(rows, cols, &shm_ids[0]);
	int **matrixb = create_and_fill_shared_matrix(rows, cols, &shm_ids[1]);
	int **matrixres = create_shared_matrix(rows, cols, &shm_ids[2]);

	printf("\nLos valores generados aleatoriamente para las matrices A y B (respectivamente) son:\n\n");
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			printf("%d ", matrixa[row][col]);
		}
		printf("\t\t");
		for (int col = 0; col < cols; col++) {
			printf("%d ", matrixb[row][col]);
		}
		printf("\n");
	}
	printf("\n\n");

	/* int num_hijos = ceil((double)n / 2);
	 */

    int num_hijos;
    if(n%2==0) num_hijos = n/2;
	else num_hijos = (n/2)+1;

    printf("Cantidad de hijos %d", num_hijos);
    for (i = 0; i < num_hijos; i++) if (!fork()) break;

	if (i == num_hijos) {
		// Estoy en el padre, quién imprimirá la matriz resultado
		for (int s = 0; s < num_hijos; ++s) wait(NULL);
		printf("\n\n");
		printf("[%d] - all childs finished. Printing result values in parent\n\n", getpid());
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				int res = matrixres[row][col];
				printf(res < 1000 ? "%d  " : "%d ", res);
			}
			printf("\n");
		}
		shmdt(matrixa);
		shmdt(matrixb);
		shmdt(matrixres);
		for (int s = 0; s < 3; s++) shmctl(shm_ids[s], IPC_RMID, 0);
	} else {
		// Estoy en el i-esimo hijo
		printf("[%d] - child %d began to process ring segment between indexes [%d, %d]\n", getpid(), i, i, n - 1 - i);
		for (int col = i; col < n - i; col++) {
			matrixres[i][col] = get_mult_result(matrixa, matrixb, n, i, col);
			matrixres[n - 1 - i][col] = get_mult_result(matrixa, matrixb, n, n - 1 - i, col);
		}

		for (int row = i; row < n - i; row++) {
			matrixres[row][i] = get_mult_result(matrixa, matrixb, n, row, i);
			matrixres[row][n - 1 - i] = get_mult_result(matrixa, matrixb, n, row, n - 1 - i);
		}

		shmdt(matrixa);
		shmdt(matrixb);
		shmdt(matrixres);
		printf("[%d] - child %d finished processing ring segment\n", getpid(), i);
	}
	
	return EXIT_SUCCESS;
}

int **create_shared_matrix(int rows, int cols, int *shm_id) {
	size_t sizeMatrix = sizeof_shmt(rows, cols, sizeof(int));
	*shm_id = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | S_IRUSR | S_IWUSR);
	int **matrix = (int **) shmat(*shm_id, NULL, 0);
	create_index_segment((void*)matrix, rows, cols, sizeof(int));
	return matrix;
}

int **create_and_fill_shared_matrix(int rows, int cols, int *shm_id) {
	int **matrix = create_shared_matrix(rows, cols, shm_id);
	fill_matrix(rows, cols, matrix);
	return matrix;
}

void fill_matrix(int rows, int cols, int **matrix) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			matrix[row][col] = (rand() % 10) + 10;
		}
	}
}

unsigned int sizeof_shmt(int rows, int cols, size_t sizeElement) {
	size_t size = rows * sizeof(void *); // size of the index segment
	size += (cols * rows * sizeElement); // size of the data segment
	return size;
}

void create_index_segment(void **m, int rows, int cols, size_t sizeElement) {
	// size of a row in our matrix
	// is the number of columns multiplied by the size of the data type
	size_t sizeRow = cols * sizeElement;
	// Then the first position in our linear vector will be
	// the address of m plus the number of rows
	// that means that now m[0] points to the first row in our matrix
	m[0] = m + (rows * sizeof(void *));
	for (int i = 1; i < rows; i++) {
		m[i] = (m[i - 1] + sizeRow);
	}
}

int get_mult_result(int **a, int **b, int n, int i, int j) {
	int res = 0;
	for (int k = 0; k < n; k++) {
		res += a[i][k] * b[k][j];
	}
	return res;
} 

void throw_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}
