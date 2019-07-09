
	/* p2ejercicio7.c
	Programa que crea hilos concurrentes de cliente y proveedor,
	cada uno decrementa o incrementa respectivamente un número de un elemento del buffer de camisetas
	selecionado de forma aleatoria con exclusión mutua mediante un semáforo binario. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

//Función que muestra por pantalla un vector de enteros.
void escribeVector(int *camisetas, int longitud){

	int i;

	for(i=0; i<longitud; i++)
		printf(" %i", camisetas[i]);
	printf("\n");
	}

// Función a la que llaman los hilos proveedores.
void * proveedores(void *camisetas){

	int modelo = rand()%5, cantidad = rand()%10;
	int *cam=(int*)camisetas;

	if(pthread_mutex_lock(&mtx) != 0)
		printf("Error en mutex_lock...\n");

	cam[modelo]=cam[modelo]+cantidad;

	if(pthread_mutex_unlock(&mtx) != 0)
		printf("Error en mutex_unlock...\n");

	pthread_exit(NULL);
	}

//Función a la que llaman los hilos clientes.
void * clientes(void *camisetas){

	int modelo=rand()%5+1, cantidad=rand()%10;
	int *cam=(int*)camisetas;

	if(pthread_mutex_lock(&mtx) != 0)
		printf("Error de mutex_lock...\n");

	cam[modelo]=cam[modelo]-cantidad;

	if(pthread_mutex_unlock(&mtx) != 0)
		printf("Error de mutex_unlock...\n");

	pthread_exit(NULL);
	}

int main(){

	int camisetas[5]={1,2,3,4,5}, N=5, M=5, i, j;
	pthread_t proveedores_thread[5], clientes_thread[5];

	srand(time(NULL));

	printf("Vector inicial:\n");
	escribeVector(camisetas, 5);

	// Se crean los hilos clientes y proveedores.
	for(i=0; i<M; i++)
		if(pthread_create(&proveedores_thread[i], NULL, &proveedores, (void*)camisetas)){
			printf("Error: No se pudieron crear los hilos.\n");
			return 1;
			}
	for(j=0; j<N; j++)
		if(pthread_create(&clientes_thread[j], NULL, &clientes, (void*)camisetas)){
			printf("Error: No se pudieron crear los hilos.\n");
			return 1;
			}

	// Se juntan los hilos clientes y proveedores.
	for(i=0; i<M; i++)
		if(pthread_join(proveedores_thread[i], NULL)){
			printf("Error: No se pudieron juntar los hilos.\n");
			return 1;
			}
	for(j=0; j<N; j++)
		if(pthread_join(clientes_thread[j], NULL)){
			printf("Error: No se pudieron juntar los hilos.\n");
			return 1;
			}

	printf("Vector final:\n");
	escribeVector(camisetas, 5);
	pthread_exit(NULL);
	}

