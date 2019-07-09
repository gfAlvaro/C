
/*	p2ejercicio1.c
	Programa que crea dos hebras en el que cada una muestra por pantalla una parte del mensaje.*/

#include <pthread.h> 
#include <stdio.h>

void primeraFuncion(char cadena[]){

	printf(" %s ", cadena );
	}

void * segundaFuncion(void *cadena){

	printf("%s \n", (char*)cadena);
    
	pthread_exit(NULL);
	}

int main(){

	pthread_t segundaFuncion_thread;

	if(pthread_create(&segundaFuncion_thread, NULL, &segundaFuncion, "mundo")){
		fprintf(stderr, "Error creando hebra\n");
		return 1;
		}

	primeraFuncion("Hola");

	if(pthread_join(segundaFuncion_thread, NULL)){
		fprintf(stderr, "Error al juntar hilos\n");
		return 2;
		}

	pthread_exit(NULL);
	}
