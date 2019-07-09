
/*	p2ejercicio5.c
	Programa que realiza el producto de una matriz de 3x3 y un vector 3x1,
	creando un hilo para cada fila de la matriz que la multiplica por el vector dado.*/

#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>

struct vectores{
	int vector1[3];
	int vector2[3];
	};

// Función que muestra por pantalla una matriz de nueve enteros.
void escribeMatriz(int matriz[3][3]){

	int i, j;
	
	for(i=0; i<3; i++){
		for(j=0; j<3; j++)
			printf(" %i", matriz[i][j]);
		printf("\n");
		}
	}
	
// Función que muestra por pantalla un vector de tres enteros.
void escribeVector(int vector[3]){

	int i;

	for(i=0; i<3; i++)
		printf(" %i", vector[i]);
	printf("\n");
	}

// Función que copia un vector de tres enteros.
void copiaVector(int destino[3], int origen[3]){

	int i;

	for(i=0; i<3; i++)
		destino[i]=origen[i];
	}

//Función a la que llaman los hilos creados y que multiplica una fila de la matriz por el vector.
void * sumaFila(void * vectores){

	int i;
	long int aux=0;

	printf("Fila matriz:\n");
	escribeVector(((struct vectores*) vectores)->vector1);
	for(i=0; i<3; i++)
		aux = aux + ((struct vectores*) vectores)->vector1[i] * ((struct vectores*) vectores)->vector2[i];
	printf("Suma: %li\n", aux);
	pthread_exit((void*)aux);
	}

int main(){

	int i;
	int matriz[3][3]={{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, vector[3]={10, 11, 12};
	int vFinal[3];
	pthread_t sumaFila_thread[3];
	struct vectores aux[3];

	printf("Matriz:\n");
	escribeMatriz(matriz);
	printf("Vector:\n");
	escribeVector(vector);
	printf("\n");

	// Se crean los hilos y se les asigna una fila de la matriz y el vector a multiplicar.
	for(i=0; i<3; i++){

		copiaVector(aux[i].vector1, matriz[i]);
		copiaVector(aux[i].vector2, vector);

		printf("Hilo: %i Fila Matriz: \n", i);
		escribeVector(aux[i].vector1);
		printf("Hilo: %i Vector: \n", i);
		escribeVector(aux[i].vector2);

		if(pthread_create(&sumaFila_thread[i], NULL, &sumaFila, (void*)&aux[i])){
			fprintf(stderr, "Error al crear la hebra.\n");
			return 1;
			}
		}

	// Se unen los hilos y se muestra el resultado.		
	for(i=0; i<3; i++)
		if(pthread_join(sumaFila_thread[i], (void*)&vFinal[i])){
			fprintf(stderr, "Error al juntar las hebras.\n");
			return 1;
		}
	
	printf("Vector resultante:\n");
	escribeVector(vFinal);

	pthread_exit(NULL);
	}

