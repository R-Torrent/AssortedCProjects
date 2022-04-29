/* ejercicio 7_8 */

#include <stdio.h>
#include <string.h>

#define MAXLINE 80
#define LINESPERPAGE 50 /* incluye el encabezado */

int main(int argc, char *argv[])
{
	char line[MAXLINE];
	unsigned int page, lineno;
	FILE *fp;

	while(--argc){
		if(!(fp = fopen(*++argv, "r"))){
			fprintf(stderr, "Error: no se puede abrir el archivo %s\n", *argv);
			continue;
		}
		page = 1, lineno = 1;
		while(fgets(line, MAXLINE, fp)){
			if(lineno == 1)
				printf("\n*** %s: pagina = %u ***\n", *argv, page);
			else
				printf("%s%s", line, line[strlen(line)-1] != '\n' ? "\n" : "");
			if(lineno++ == LINESPERPAGE)
				page++, lineno = 1;
		}
		fclose(fp);
	}
	return 0;
}