/* ejercicio 7_1 */

/* opciones de entrada:
	'may': minusculas -> mayusculas
	'min': mayusculas -> minusculas */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

main(int argc, char *argv[])
{
	int c;
	
	if (argc == 2){
		if(strcmp(argv[1], "min") == 0)
			while((c = getchar()) != EOF)
				putchar(tolower(c));
		else if(strcmp(argv[1], "may") == 0)
			while((c = getchar()) != EOF)
				putchar(toupper(c));
		else{
			printf("Error: modificador %s no valido\n", argv[1]);
			goto error;
		}
		return 0;
	}
	printf("Error: sintaxis no valida\n");
error:
	printf("Opciones validas: \'min\' y \'may'\n");

	return -1;
}