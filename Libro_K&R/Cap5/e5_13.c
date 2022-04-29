/* ejercicio 5_13 */

/* tail -n: imprime las ultimas n lineas de la entrada */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 10			/* numero de lineas por omision */
#define MAXLINES 5000	/* max # de lineas por ordenar */

char *lineptr[MAXLINES];	/* apuntadores a lineas de texto */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

main(int argc, char *argv[])
{
	int nlines;		/* numero de lineas de entrada leidas */
	int n_tail = N;

	if(!--argc || (argc == 1 && **++argv == '-')){
		if(argc)
			n_tail = atoi(*argv + 1);
	}
	else{
		printf("Uso: tail -n [%d]\n", N);
		return -1;
	}
	
	if((nlines = readlines(lineptr, MAXLINES)) >= 0){
		if(nlines < n_tail)
			writelines(lineptr, nlines);
		else
			writelines(lineptr + nlines - n_tail, n_tail);
		return 0;
	}
	else{
		printf("Error: entrada demasiado grande\n");
		return -1;
	}

	return 0;
}

/* getline: lee una linea en s, regresa su longitud */
int getline(char s[], int lim)
{
	int c, i;

	for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
		s[i] = c;
	if(c == '\n'){
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}

#define MAXLEN 1000		/* max longitud de cualquier linea de entrada */

int getline(char line[], int maxline);
char *alloc(int);

/* readlines: lee lineas de entrada */
int readlines(char *lineptr[], int maxlines)
{
	int len, nlines;
	char *p, line[MAXLEN];
	
	nlines = 0;
	while((len = getline(line, MAXLEN)) > 0)
		if(nlines >= maxlines || (p = alloc(len)) == NULL)
			return -1;
		else{
			line[len-1] = '\0';	/* elimina caracter nueva linea */
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	return nlines;
}

/* writelines: escribe lineas de salida */
void writelines(char *lineptr[], int nlines)
{
	int i;

	for(i = 0; i < nlines; i++)
		printf("%s\n", lineptr[i]);
}

#define ALLOCSIZE 10000	/* tamano del espacio disponible */

static char allocbuf[ALLOCSIZE];	/* almacenamiento para alloc */
static char *allocp = allocbuf;		/* siguiente posicion libre */

/* alloc: regresa un apuntador a n caracteres */
char *alloc(int n)
{
	if(allocbuf + ALLOCSIZE - allocp >= n){	/* si cabe */
		allocp += n;
		return allocp - n;	/* antigua p */
	}
	else	/* no hay suficiente espacio */
		return 0;
}