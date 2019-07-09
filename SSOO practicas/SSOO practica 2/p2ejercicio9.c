
#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t cerrojo = PTHREAD_MUTEX_INITIALIZER;
char buffer[1024];
char fichero[100];
int copias;

void erroresEntrada(int argc, char **argv){

	int num=atoi(argv[2]);

	if(argc!=3){
		printf("Error: número de parámetros incorrecto.\n");
		exit(EXIT_SUCCESS);
		}

	if(num<1){
		printf("Error: segundo parámetro fuera de rango.\n");
		exit(EXIT_SUCCESS);
		}
	}

void *leeFichero(){

	FILE *f;

	char *c;

	if((f=fopen(fichero, "r")) == NULL){		
		fprintf(stderr, "Error: el fichero %s no existe", fichero);
		exit(EXIT_SUCCESS);
		}
	//Entrar sección crítica.
	if(pthread_mutex_lock(&cerrojo) != 0)
		printf("Error en mutex_unlock...\n");

	while((c=fgets(buffer,1024, f)) != NULL){

		//Salir sección crítica.
		if(pthread_mutex_unlock(&cerrojo) != 0)
			printf("Error en unmutex_lock...\n");

		if(c != NULL)
			sleep(1);
			//Entrar sección crítica.
			if(pthread_mutex_lock(&cerrojo) != 0)
				printf("Error en mutex_unlock...\n");
		}

	if (ferror(f)){
		perror(fichero);
		exit(EXIT_FAILURE);
		}
	fclose(f);
	pthread_exit(NULL);
	}

void *copiaFichero(void * i){


	FILE *f[copias];
	char num[100];
	int *c=(int*)i;
	int cuenta=*c;
	char fich[copias][100];

	sprintf(num, "%i", cuenta);
	strcpy(fich[cuenta], fichero);
	strcat(fich[cuenta], num);

	if((f[cuenta]=fopen(fich[cuenta], "w")) == NULL){
		fprintf(stderr, "Error: el fichero %s no existe", fichero);
		exit(EXIT_SUCCESS);
		}

	while(1){
		sleep(1);
		// Entrar sección crítica.
		if(pthread_mutex_lock(&cerrojo) != 0)
			printf("Error en mutex_lock...\n");

		fputs(buffer, f[cuenta]);
		printf("%s",buffer);
		printf("hilo %i\n", cuenta);

		// Salir sección crítica.
		if(pthread_mutex_unlock(&cerrojo) != 0)
			printf("Error en mutex_unlock...\n");
		if(feof(f[cuenta]))
			break;
		}

	if (ferror(f[cuenta])){
		perror(fich[cuenta]);
		exit(EXIT_FAILURE);
		}

	fclose(f[cuenta]);
	printf("Me fui\n");
	pthread_exit(NULL);
	}

int main(int argc, char*argv[]){

	erroresEntrada(argc, argv);

	copias=atoi(argv[2]);
	strcpy(fichero, argv[1]);
	pthread_t leeFichero_thread, copiaFichero_thread[copias];
	int i;
	int veces[100];

	if(pthread_create(&leeFichero_thread, NULL, leeFichero, NULL)){
		printf("Error al crear hilo de lectura\n");
		return 1;	
		}
	for(i=0; i<copias; i++){
		veces[i]=i;
		if(pthread_create(&copiaFichero_thread[i], NULL, copiaFichero, (void*)&veces[i])){
			printf("Error al crear hilo de copiado\n");
			return 1;	
			}
		}

	if(pthread_join(leeFichero_thread, NULL)){
		printf("Error al juntar hilo de lectura\n");
		return 1;	
		}
	for(i=0; i<copias; i++)
		if(pthread_join(copiaFichero_thread[i], NULL)){
			printf("Error al juntar hilo de copiado\n");
			return 1;	
			}

	pthread_exit(NULL);
	}

