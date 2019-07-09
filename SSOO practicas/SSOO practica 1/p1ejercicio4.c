
/*	p1ejercicio4.c
	Programa que crea dos procesos, siendo que el primero llama a la calculadora gnome 
	y el segundo llama a gedit y abre los archivos pasados como parámetros en la línea de comandos. */

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

void llamaCalculadora(char *argv[]){
	if(execlp(argv[1], argv[1], NULL)==-1){
		perror("Falla en la ejecucion gnome-calculator");
		printf("errno value= %d\n", errno);
		exit(EXIT_FAILURE);
		}
	}

void llamaEditor(char *argv[]){

	if(execvp(argv[2], &argv[2]) < 0 ){
		perror("exec");
		printf("errno value= %d\n", errno);
		exit(EXIT_FAILURE);
		}		
	}

int main(int argc, char *argv[]){
	int i;
	int status;
	int childpid;
	pid_t rf;
    
	for(i=0; i < 2; i++){
		rf = fork();
		if(rf==0)
			break;
		}
		
	switch(rf){
		case -1:
			perror("fork error");
			printf("errno value= %d \n", errno); 
			exit(EXIT_FAILURE);
		case 0:
			if(i==0)
				llamaCalculadora(argv);
			if(i==1)
				llamaEditor(argv);
			exit(0);
		default:
			if(i==0)
				printf("\npadre con pid: %ld \n\n", (long)getpid());
		}
	for(i=0; i < 2; i++){
		sleep(2);
		childpid = wait(&status);
		comprobarProceso(childpid, status);
		}
	printf ("\nFinal de ejecucion... ");
	exit(EXIT_SUCCESS);
	}
