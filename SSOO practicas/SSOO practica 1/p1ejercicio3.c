
/*	p1ejercicio3.c
	Programa que crea el árbol de procesos mostrado en la relación de ejercicios de la práctica 1,
	cada proceso espera por el final de sus hijos, almacena la suma de los últimos dígitos de los PID de estos
	y después le suma el último dígito del PID del padre. */

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

	int i, j;
	int status;
	int childpid, suma=0;
	pid_t rf;

	for(i=0; i<2; i++){
		if((rf=fork()) == -1){
			perror("fork error");
			printf("errno value= %d\n", errno); 
			exit(EXIT_FAILURE);
			}
		if(rf == 0)
			break;
		}
	if(rf!=0){ //Proceso padre
		for(i=0; i<2; i++){
			sleep(5);
			childpid=wait(&status);
			comprobarProceso(childpid, status);
			suma = suma + WEXITSTATUS(status);
			}
		suma = suma + getpid()%10;
		printf("Soy el padre (PID=%d) y mi suma es %d\n", getpid(), suma);
		printf ("\nFinal de ejecucion...");
		exit(EXIT_SUCCESS);
		}
	//procesos hijos
	else if(i==0){ //primer hijo
		suma = getpid()%10;
		printf("Soy el primer hijo (PID=%d) y mi suma es %d\n", getpid(), suma);
		exit(suma);
		}
	else //segundo hijo
		for(j=0; j<2; j++){
			if((rf=fork()) == -1){
				perror("fork error");
				printf("errno value= %d\n", errno); 
				exit(EXIT_FAILURE);
				}
			if(rf == 0)
				break;
				}
		if(rf!=0){
			for(j=0; j<2; j++){
				sleep(5);
				childpid = wait(&status);
				comprobarProceso(childpid, status);
				suma = suma + WEXITSTATUS(status);
				}
			suma = suma + getpid()%10;
			printf("Soy el segundo hijo (PID=%d) y mi suma es %d\n", getpid(), suma);
			exit(suma);
			}
			
		else if(j==0){ //primer nieto
			suma = getpid()%10;
			printf("Soy el primer nieto (PID=%d) y mi suma es %d\n", getpid(), suma);
			exit(suma);
			}
		else{ //segundo nieto				
			if((rf=fork()) == -1){
				perror("fork error");
				printf("errno value= %d\n", errno);
				exit(EXIT_FAILURE);
				}
			if(rf!=0){
				sleep(5);
				childpid = wait(&status);
				comprobarProceso(childpid, status);
				suma = WEXITSTATUS(status) + getpid()%10;
				printf("Soy el segundo nieto (PID=%d) y mi suma es %d\n", getpid(), suma);
				exit(suma);
				}
			else{ //bisnieto
				suma = getpid()%10;
				printf("Soy el bisnieto (PID=%d) y mi suma es %d\n", getpid(), suma);
				exit(suma);
				}
			}
	return 0;
	}
