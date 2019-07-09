struct entrada {
	int *vector;
	int longitud;
};

program CALCULADORA {
	version CALCULADORA_VER {
		int media(entrada) = 1;
		int maximo(entrada) = 2;
		int factorial(entrada) = 3;
	} = 1;
} = 0x30000001;
