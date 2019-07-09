
/*	p1ejercicio8.c
	Programa que crea varios procesos hijos y un espacio de memoria compartida por todos ellos,
	en la que los hijos escribirán varias veces cuando el proceso padre les dé permiso para ello,
	mediante una estrategia de paso de testigo. */

#include <sys/types.h> // Para uso del tipo pid_t
#include <unistd.h> // Para uso de la función fork()
#include <stdio.h> // E/S estándar
#include <stdlib.h> // Para uso de macros EXIT_FAILURE y EXIT_SUCCESS
#include <errno.h> // Para tratamiento de errores
#include <sys/wait.h> // Para uso de la funcion wait()
#include <sys/shm.h> // Para uso de memoria compartida
#define LLAVE (key_t)  234 // Clave de acceso

// Función que comprueba los posibles estados y errores de un proceso hijo.
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

	int i, num, pid, *memoria=NULL;
	int status;
	int childpid;
	pid_t rf;
	int ID_memoria;

	//Conseguimos ID para memoria compartida.
	if((ID_memoria=shmget(LLAVE, sizeof(int)*100, 0777 | IPC_CREAT)) == -1){
		printf(" Error al conseguir Id para memoria compartida. \n");
		exit(0);
		}
	//Conseguimos memoria compartida.
	if((memoria=(int*)shmat(ID_memoria, (char*)0, 0)) == NULL){
		printf(" Error al conseguir memoria compartida. \n");
		exit(0);
		}

	memoria[0]=0;
	memoria[1]=-1;

	do{
	    	printf("Introduzca el número de procesos a crear. (Debe ser mayor que 0.) \n");
	    	scanf("%i", &num);
		}while(num<=0);

	for(i=0; i<num; i++){
		if((rf = fork())==-1){
			perror("fork error");
			printf("errno value= %d \n", errno); 
			exit(EXIT_FAILURE);
			}
		else if(rf==0){
			pid = getpid();
			//Se almacenan los PID de los hijos que usarán memoria compartida.
			memoria[2+i]=pid;
			break;
			}
		}

	if(rf==0){ // Procesos hijos.

		printf("Proceso hijo %d, padre = %d \n", pid, getppid());
		
		// El hijo espera que se le conceda permiso para usar la memoria compartida.
		while(memoria[1]!=pid)
			sleep(1);
			
		// El hijo escribe en la memoria compartida.
		for(i=0; i<100; i++){
			memoria[0]=memoria[0]+1;
			printf("Entro %i\n", i);
			}
			
		// El hijo avisa de que deja de usar la memoria compartida.
		memoria[1]=-1;
		exit(0);
		}
	else{ // Proceso padre.
		printf("\nPadre con pid: %ld \n", (long)getpid());
		
		for(i=0; i<num; i++){
			// El padre da permiso a un hijo para usar memoria compartida.
			memoria[1]=memoria[2+i];
			
			// El padre espera a que un hijo deje de usar la memoria compartida para dar permiso a otro.
			while(memoria[1]!=-1)
				sleep(1);
			}
			
		for(i=0; i<num; i++){
			childpid=wait(&status);
			comprobarProceso(childpid, status);
			}

		printf("Valor de variable: %i \n", memoria[0]);

		// Liberamos la memoria compartida
		if(ID_memoria != -1)
			shmdt((char*)memoria);
		printf ("\nFinal de ejecucion... ");
		exit(EXIT_SUCCESS);
		}
	}
