#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

/*
 * Estructura que representa la información requerida por cada hilo para operar
 */
typedef struct{
    int *vector_a;
    int *vector_b;
    int *vector_result;
    char *tipo_hilo;
    int init;
    int final;
}VectorWrapper;



/*
 *Variables globales
 */
int *vector_a = NULL, *vector_b = NULL, *vector_result = NULL;

/*
 * Función que se encarga de pedir el numero de hilos al usuario
 * @param longitud_vector: longitud del (los) vector(es) a operar
 */
int get_number_threads(int longitud_vector) {
    int number_threads = 0;

    do {
        printf("\nIngrese el numero de hilos adicionales: ");
        scanf("%d",&number_threads);

        if (longitud_vector <= number_threads) {
            printf("\nError: El numero de hilos adicionales sumado al principal debe ser menor o igual al numero de elementos del vector, es decir %d <= %d.\n",(number_threads+1), longitud_vector);
        }
    } while (number_threads < 0 || longitud_vector <= number_threads);

    return number_threads;
}

/*
 * Función que lee vector_a y vector_b desde y la longitud de estos desde el archivo
 * @param file_name nombre del archivo
 * @param longitud_vector longitud del vector
 */
void read_vectors(char *dir_file, int *longitud) {
    
    
    FILE *fp = fopen(dir_file, "r");
    fscanf(fp,"%d", longitud);

    vector_a = (int*)malloc(*longitud*sizeof(int));
    vector_b = (int*)malloc(*longitud*sizeof(int));

    for(int i = 0; i < *longitud*2; i++) {
        if(i < *longitud) {
            fscanf(fp,"%d", &vector_a[i]);
        } else {
            fscanf(fp,"%d", &vector_b[i-*longitud]);
        }
    }

    fclose(fp);
}

/*
 * Función que se encarga de multiplicar los elementos de dos vectores
 * @param void *wrapper: puntero a la estructura que contiene los vectores a multiplicar y los limites
 * @return void*: no retorna nada
 */
void* vector_multiplier(void *arg) {

    VectorWrapper *parameter = (VectorWrapper*)arg;

    int aux_multiplier;

    printf("\nHilo %ld - %s de [ %d - %d ]", pthread_self(), parameter->tipo_hilo, parameter->init, parameter->final);

    for (int i = parameter->init; i < parameter->final; i++) {
        aux_multiplier = parameter->vector_a[i]*parameter->vector_b[i];
        parameter->vector_result[i] = aux_multiplier;
    }

    return NULL;
}

/*
 * Función que imprime un vector
 * @param vector: puntero al vector a imprimir
 * @param longitud: longitud del vector
 */
void print_vector(int *vector, int longitud) {
    printf("\n[");
    for(int i = 0; i < longitud; i++) {
        printf("%d, ", *(vector + i));
    }
    printf("]\n");
}

/*
 * Funcion que suma los componenetes de un vector
 * @param vector: puntero al vector a sumar
 * @param longitud: longitud del vector
 */
int sum_vector(int *vector, int longitud) {
    int sum = 0;
    for(int i = 0; i < longitud; i++) {
        sum += *(vector + i);
    }
    return sum;
}


/*
 * Función principal
 */
int main(int agrc, char *agrv[]) {

    int longitud_vector;
    int number_threads;
    pthread_t *threads;
    VectorWrapper *parameter_hs, *parameter_hp;
    int rango_threads;
    char *dir_file = "input3.txt";

    printf("\nPrograma que realiza el producto escalar entre dos vectores");
    printf("con operaciones ejecutadas de manera paralela.\n\n");


    read_vectors(dir_file, &longitud_vector);


    number_threads = get_number_threads(longitud_vector);
    
    vector_result = (int*)malloc(longitud_vector*sizeof(int));
    threads = (pthread_t*)malloc(number_threads*sizeof(pthread_t));

    rango_threads = (int)longitud_vector/(number_threads + 1); //Agregando uno para que el ultimo hilo tenga el resto

    printf("\nParticiones de: %d \n\n", rango_threads);
    
    for (int i = 0; i < number_threads; i++) {
        parameter_hs = (VectorWrapper*)malloc(sizeof(VectorWrapper));
        parameter_hs->vector_a = vector_a;
        parameter_hs->vector_b = vector_b;
        parameter_hs->vector_result = vector_result;
        parameter_hs->init = i*rango_threads;
        parameter_hs->final = parameter_hs->init + rango_threads;
        parameter_hs->tipo_hilo = "secundario";
        pthread_create(&threads[i], NULL, vector_multiplier, (void*)parameter_hs);        
    }

    if (rango_threads*number_threads < longitud_vector) {
        parameter_hp = (VectorWrapper*)malloc(sizeof(VectorWrapper));
        parameter_hp->vector_a = vector_a;
        parameter_hp->vector_b = vector_b;
        parameter_hp->vector_result = vector_result;
        parameter_hp->init = parameter_hs->final;
        parameter_hp->final = longitud_vector;
        parameter_hp->tipo_hilo = "primario";
        vector_multiplier((void*)parameter_hp);
    }

    for (int  j = 0; j < number_threads; j++) {
        pthread_join(threads[j], NULL);
    }

    printf("\n\nVector A: ");
    print_vector(vector_a, longitud_vector);
    printf("\nVector B: ");
    print_vector(vector_b, longitud_vector);

    printf("\n\nEl resultado del producto punto es: %d\n", sum_vector(vector_result, longitud_vector));

    return EXIT_SUCCESS;
}