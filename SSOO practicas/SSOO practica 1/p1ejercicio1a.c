
/*	p1ejercicio1a.c
	Programa que crea un número de procesos igual al introducido por teclado,
	los procesos son creados de forma que todos son hijos del proceso inicial. */

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

int main(){
	int i, num;
	int status;
	int childpid;
	pid_t rf;
    
	do{    
		printf("Introduzca el número de procesos a crear. (Mayor que 0) \n");
		scanf("%i", &num);
    		}while(num<=0);
    
    	for(i=0; i < num; i++){
		rf = fork();
		if(rf==0)
			break;
		}

	switch(rf){
		case -1:
			perror("fork error");
			printf("errno value= %d \n", errno); 
			exit(EXIT_FAILURE);
		case 0: // Proceso hijo.
			printf("Soy el hijo %d, mi PID es %d y mi padre es %d \n", (i+1), getpid(), getppid());
			exit(0);				
		default: //Proceso padre.
			printf("\npadre con pid: %ld \n\n", (long)getpid());
				
			for(i=0; i < num; i++){
				childpid = wait(&status);
				comprobarProceso(childpid, status);
				}
			printf("No hay más hijos que esperar. \n");
			printf ("\nFinal de ejecucion... ");
			exit(EXIT_SUCCESS);
		}
	}
