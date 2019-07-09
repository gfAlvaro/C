
/*	p2ejercicio3.c
	Programa que genera un vector de enteros aleatorios,
	luego crea un número de hilos pasado como parámetro en línea de órdenes
	y les asigna un conjunto de elementos del vector para sumarlos de la forma más equitativa posible.
	Al juntar los hilos sumará las sumas parciales de cada hilo, mostrando el resultado al final. */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estructura que almacenará cada subvector que será asignado a los hilos.
struct vector{
	int *vector;
	int longitud;
	};

// Función para comprobar errores en la entrada de parámetros en línea de comandos.
void erroresEntrada(int argc, int nHilos){
	
	if(argc!=2){
		printf("Error: número de argumentos incorrecto.\n");
		exit(EXIT_SUCCESS);
		}		
	if((nHilos<1) || (nHilos>10)){
		printf("Error: número de hilos fuera de rango.\n");
		exit(EXIT_SUCCESS);
		}
	}

// Función para mostrar un vector por pantalla.
void escribeVector(int * vector, int longitud){
	
	int i;
	for(i=0; i<longitud; i++)
		printf(" %i", vector[i]);
	printf("\n");
	}

// Función a la que llaman los hilos creados para sumar los elementos del vector.
void * sumaVector(void * vector){
	
	long int suma=0;
	int i;
	
	for(i=0; i<((struct vector*) vector)->longitud; i++)
		suma = suma + ((struct vector*)vector)->vector[i];
	
	pthread_exit((void*)suma);
	}

int main(int argc, char* argv[]){
	
	pthread_t sumaVector_thread[100];
	int vector[10], vecAux[10][10];
	struct vector Aux[10];
	int nHilos=atoi(argv[1]), sumaTotal=0, i, j, resto, cociente;
	long int suma;
	
	erroresEntrada(argc, nHilos);
		
	// Se generan números aleatorios para almacenar en el vector.
	srand(time(NULL));
	for(j=0; j<10; j++)
		vector[j] = rand()%10;
	printf("Vector aleatorio:");
	escribeVector(vector, j);
	
	// Sistema para crear los hilos y asignarles los elementos del vector a sumar.
	resto = 10%nHilos;
	cociente = 10/nHilos;
	
	for(i=0; i<nHilos; i++){
		if((resto!=0) && (i<resto))
			for( j=0; j<=cociente; j++)
				vecAux[i][j]=vector[j+(i*(cociente+1))];
		else
			for( j=0; j<cociente; j++)
				vecAux[i][j]=vector[j+(i*cociente+resto)];
		
		Aux[i].vector=vecAux[i];
		Aux[i].longitud=j;
		
		if(pthread_create(&sumaVector_thread[i], NULL, &sumaVector, (void*)&(Aux[i]))){
			fprintf(stderr, "Error creando hebras.\n");
			return 1;
			}
		}
	
	// El hilo de control junta los demás hilos.
	for(i=0; i<nHilos; i++){
		if(pthread_join(sumaVector_thread[i], (void*)&suma)){
			fprintf(stderr, "Error al juntar hilos.\n");
			return 2;
			}
		printf(" %iª Suma parcial = %ld\n", i+1, suma);
		sumaTotal = sumaTotal + suma;
		}
	
	printf("Suma total = %i\n", sumaTotal);	
	pthread_exit(NULL);
	}

