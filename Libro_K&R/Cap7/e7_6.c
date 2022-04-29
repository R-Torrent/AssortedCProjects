/* ejercicio 7_6 */

#include <stdio.h>
#include <string.h>

#define MAXLINE 80
int main(int argc, char *argv[])
{
	FILE *f1, *f2;
	char s1[MAXLINE], s2[MAXLINE], *prog = argv[0];	/* nombre del programa para errores */
	char *ps1, *ps2;
	int nlin = 0, dif;

	if(argc != 3){	/* faltan argumentos */
		fprintf(stderr, "Error en %s: faltan argumentos\n", prog);
		exit(1);
	}
	if(!(f1 = fopen(argv[1], "r"))){
		fprintf(stderr, "Error en %s: no se puede abrir el archivo %s\n", prog, argv[1]);
		exit(1);
	}
	if(!(f2 = fopen(argv[2], "r"))){
		fprintf(stderr, "Error en %s: no se puede abrir el archivo %s\n", prog, argv[2]);
		exit(1);
	}
	do{
		nlin++;
		dif = 1;
		ps1 = fgets(s1, MAXLINE, f1);
		ps2 = fgets(s2, MAXLINE, f2);
		if(ps1 && ps2)
			dif = strcmp(ps1,ps2);
	}while(!dif);
	if(feof(f1) && feof(f2))
		printf("%s y %s coinciden\n", argv[1], argv[2]);
	else{
		if(ps1)
			printf("Linea %d de %s: %s%s",nlin, argv[1], s1, s1[strlen(s1)-1] != '\n' ? "\n" : "");
		else if(feof(f1))
			printf("Fin de archivo %s\n", argv[1]);
		else
			printf("Linea %d de %s: Error de lectura\n", nlin, argv[1]);
		if(ps2)
			printf("Linea %d de %s: %s%s",nlin, argv[2], s2, s2[strlen(s2)-1] != '\n' ? "\n" : "");
		else if(feof(f2))
			printf("Fin de archivo %s\n", argv[2]);
		else
			printf("Linea %d de %s: Error de lectura\n", nlin, argv[2]);
	}
	return 0;
}