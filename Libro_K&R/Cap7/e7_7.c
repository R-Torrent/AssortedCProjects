/* ejercicio 7_7 */

#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

/* find: imprime lineas que coinciden con el patron del 1er. argumento */
int main(int argc, char *argv[])
{
	char line[MAXLINE], *patron;
	long lineno = 0;
	int c, except = 0, number = 0, found = 0;
	FILE *fp;

	while(--argc > 0 && (*++argv)[0] == '-')
		while(c= *++argv[0])
			switch(c){
			case 'x':
				except = 1;
				break;
			case 'n':
				number = 1;
				break;
			default:
				fprintf(stderr, "find: opcion ilegal %c\n", c);
				argc = 0;
				found = -1;
				break;
			}
	if(argc < 1)
		fprintf(stderr,"Uso: find [-x] [-n] patron [lista de archivos]\n");
	patron = *argv;
	if(!--argc) /* entrada estándar */
		while(fgets(line, MAXLINE, stdin)){
			lineno++;
			if((strstr(line, patron) != NULL) != except){
				if(number)
					printf("%ld: ", lineno);
				printf("%s%s", line, line[strlen(line)-1] != '\n' ? "\n" : "");
				found++;
			}
		}
	else
		while(argc--){
			if(!(fp = fopen(*++argv, "r"))){
				fprintf(stderr, "Error: no se puede abrir el archivo %s\n", *argv);
				exit(1);
			}
			lineno = 0;
			while(fgets(line, MAXLINE, fp)){
				lineno++;
				if((strstr(line, patron) != NULL) != except){
					printf("%s", *argv);
					if(number)
						printf(" %ld", lineno);
					printf(": %s%s", line, line[strlen(line)-1] != '\n' ? "\n" : "");
					found++;
				}
			}
			if(!feof(fp)){
				fprintf(stderr, "Error de lectura en el archivo %s\n", *argv);
				exit(1);
			}
			fclose(fp);
		}
	return found;
}