
/*	p2ejercicio2.c
	Programa que crea N hebras las cuales realizan la suma de dos números aleatorios 
	y luego suma los resultados de cada hilo mostrando por pantalla las sumas parciales y la total. */

#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void * sumaRand(){

	int suma;
	
	suma = rand()%10 + rand()%10;
	pthread_exit((void*)suma);
	}

int main(){

	pthread_t sumaRand_thread[100];
	int suma, sumaTotal=0, N, i;
	
	srand(time(NULL));
	
	do{
		printf(" Introduzca el número de hebras a crear. (Debe ser mayor que 0.)\n");
		scanf("%i", &N);
		}while(N<=0);
		
	for(i=0; i<N; i++)
		if(pthread_create(&sumaRand_thread[i], NULL, &sumaRand, NULL)){
			fprintf(stderr, "Error creando hebra\n");
			return 1;
			}

	for(i=0; i<N; i++){
		if(pthread_join(sumaRand_thread[i], (void*)&suma)){
			fprintf(stderr, "Error al juntar hilos\n");
			return 2;
			}
		printf(" %i Suma parcial = %i\n", i, suma);
		sumaTotal = sumaTotal + suma;
		}
	printf("Suma total = %i\n", sumaTotal);
	pthread_exit(NULL);
	}

