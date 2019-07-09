

/*	p2ejercicio4.c
	Programa que lee las líneas de los ficheros pasados como argumentos por línea de órdenes,
	se creará un hilo por cada fichero que se encargue de contar las líneas del mismo. */

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

// Función para comprobar los errores en la entrada.
void erroresEntrada(int argc){

	if(argc<2){
		printf("Error: número de argumentos incorrecto.\n");
		exit(EXIT_SUCCESS);
		}
	}

// Función a la que llaman los hilos creados para contar las líneas de los ficheros.
void * contarFichero(void * fichero){

	long int suma=0;
	FILE * fich;
	char aux[100];

	if((fich=(FILE*)fopen((char*) fichero, "r"))==NULL){
		printf("Error: no se ha podido abrir el fichero %s\n", (char*) fichero);
		exit(EXIT_SUCCESS);
		}
	
	while(fgets(aux,100,fich)!=NULL)
		suma++;
	
	fclose(fich);
	pthread_exit((void*)suma);
	}

int main(int argc, char *argv[]){

	int suma[argc-1], i;
	pthread_t contarFichero_thread[argc-1];

	erroresEntrada(argc);

	// Se crean los hilos y se les pasa el nombre de un fichero.
	for(i=0; i<argc-1; i++)
		if(pthread_create(&contarFichero_thread[i], NULL, &contarFichero, (void*) argv[i+1])){
			fprintf(stderr, "Error al crear los hilos.\n");
			return 1;
			}

	// Se juntan los hilos.
	for(i=0; i<argc-1; i++)
		if(pthread_join(contarFichero_thread[i], (void*)&suma[i])){
			fprintf(stderr, "Error al juntar los hilos.\n");
			return 1;
			}
	// Se muestra el número de líneas de cada fichero.
	for(i=0; i<argc-1;i++)
		printf("Número de líneas del fichero %s: %i\n", argv[i+1], suma[i]);

	pthread_exit(NULL);
	}

