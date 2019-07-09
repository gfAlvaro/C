
/*	p2ejercicio6.c
	Programa que crea tres hilos en el que cada hilo se encarga de incrementar 100 veces una variable compartida. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * sumaNumero(void * contador){

	int i;
	long int *cuenta=(long int*)contador;

	for(i=0; i<100; i++)
		*cuenta=*cuenta+1;
	pthread_exit(NULL);
	}

int main(){

	// El valor de N se podría introducir como parámetro en línea de órdenes.
	int N=3, i;
	long int contador=0;
	pthread_t sumaNumero_thread[N];

	for(i=0; i<N; i++)
		if(pthread_create(&sumaNumero_thread[i], NULL, &sumaNumero, (void*)&contador)){
			printf("Error: no se han podido crear los hilos.\n");
			return 1;
			}

	for(i=0; i<N; i++)
		if(pthread_join(sumaNumero_thread[i], NULL)){
			printf("Error: no se han podido juntar los hilos.\n");
			return 1;
			}

	printf("Resultado de la cuenta: %li\n", contador);

	pthread_exit(NULL);
	}

