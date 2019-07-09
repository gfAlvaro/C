
//	p1ejercicio2.c
//	Programa que crea un proceso hijo el cual al desaparecer el padre pasa a ser controlado por init al no usar wait() en el padre.

#include <sys/types.h> // Para uso del tipo pid_t
#include <unistd.h> // Para uso de la función fork()
#include <stdio.h> // E/S estándar
#include <stdlib.h> // Para uso de macros EXIT_FAILURE y EXIT_SUCCESS
#include <errno.h> // Para tratamiento de errores
#include <sys/wait.h> // Para uso de la funcion wait()

int main(){

	pid_t rf;
    
	rf = fork( ); 
	switch(rf){
		case -1:
			perror("fork error");
			printf("errno value= %d\n", errno); 
			exit(EXIT_FAILURE);
		case 0:
			printf("Soy el hijo, mi PID es %d \n", getpid( ));
			exit(0);
		default:
			printf("Soy el padre, mi PID es %d \n", getpid( ));
			
			printf("padre con pid: %ld\n", (long)getpid());
			sleep(20); 
			exit(EXIT_SUCCESS);
		}

	printf ("\nFinal de ejecucion...");
	exit(EXIT_SUCCESS);
	}
