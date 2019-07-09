
/*	p1ejercicio5.c
	Programa que crea dos procesos hijos, que junto con el proceso padre escriben
	de forma ordenada en un archivo de texto cuyo nombre es introducido como parámetro en línea de órdenes. */

#include <sys/types.h> // Para uso del tipo pid_t
#include <unistd.h> // Para uso de la función fork()
#include <stdio.h> // E/S estándar
#include <stdlib.h> // Para uso de macros EXIT_FAILURE y EXIT_SUCCESS
#include <errno.h> // Para tratamiento de errores
#include <sys/wait.h> // Para uso de la funcion wait()

//Función que comprueba los posibles estados y errores de un proceso hijo.
void comprobarProceso(int childpid, int status){

	if(childpid>0){
		if(WIFEXITED(status))
			printf("\nchild %d exited, status=%d\n",childpid, WEXITSTATUS(status));
		else if(WIFSIGNALED(status))
			printf("child %d killed (signal %d)\n", childpid, WTERMSIG(status));
		else if(WIFSTOPPED(status))
			printf("child %d stopped (signal %d)\n", childpid, WSTOPSIG(status));
		}
	if(childpid==-1 && errno==ECHILD){
		printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
		exit(EXIT_FAILURE);
		}
	}

int main(int argc, char *argv[]){

	pid_t rf;
	int status, childpid;
	int i;
	FILE *fichero;

	if(argc<2){
		printf(" Se necesitan más argumentos. \n");
		exit(0);
		}

	if((fichero=fopen(argv[1], "w")) == NULL){
		printf(" Error: no se pudo abrir el fichero. \n");
		exit(0);
		}

	for(i=0; i<2; i++){
		rf = fork();
		if(rf == 0)
			break;
		}

	switch(rf){
		case -1:
			perror("fork error");
			printf("errno value= %d\n", errno);
			exit(EXIT_FAILURE);
		case 0:
			printf("PID = %i, padre = %i\n", getpid(), getppid());
			sleep(1);
			fputs("-----", fichero);
			exit(0);
		default:

			for(i=0; i>2; i++){
				childpid=wait(&status);
				comprobarProceso(childpid, status);
				}
			printf("PID del padre = %i \n", getpid());
			sleep(1);
			fputs("+++++", fichero);
			fclose(fichero);
			
			exit(EXIT_SUCCESS);
		}
	}
