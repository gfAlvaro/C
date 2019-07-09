/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculadora2.h"


void
calculadora_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	entrada  media_1_arg;
	int  *result_2;
	entrada  maximo_1_arg;
	int  *result_3;
	entrada  factorial_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, CALCULADORA, CALCULADORA_VER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = media_1(&media_1_arg, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = maximo_1(&maximo_1_arg, clnt);
	if (result_2 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = factorial_1(&factorial_1_arg, clnt);
	if (result_3 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	calculadora_1 (host);
exit (0);
}
