/* ejercicio 5_7 */
/* version de 'readlines', sin usar 'alloc' */

#include <stdio.h>
#include <string.h>

#define MAXLEN 1000		/* max longitud de cualquier linea de entrada */
int getline(char line[], int max);

/* readlines: lee lineas de entrada */
int readlines(char *lineptr[], int maxlines, char *arreglo, int arreglo_size)
{
	int len, nlines;
	char line[MAXLEN], *limit = p + arreglo_size;

	nlines = 0;
	while((len = getline(line, MAXLEN)) > 0)
		if(nlines >= maxlines || arreglo + len > limit)
			return -1;
		else{
			line[len-1] = '\0';	/* elimina caracter nueva linea */
			strcpy(arreglo, line);
			lineptr[nlines++] = arreglo;
			arreglo += len;
		}
	return nlines;
}

/* getline: trae linea y la pone en s, regresa su longitud */
int getline(char s[], int lim)
{
	int c, i;

	i = 0;
	while(--lim > 0 && (c=getchar()) != EOF && c != '\n')
		s[i++] = c;
	if(c == '\n')
		s[i++] = c;
	s[i] = '\0';
	return i;
}