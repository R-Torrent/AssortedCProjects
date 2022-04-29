/* ejercicios 5_14 hasta 5_17 */

/* version del ejemplo de la seccion 5.11 del libro */

#include <stdio.h>
#include <string.h>

#define SI 1
#define NO 0
#define MAXCAMPOS 5				/* max # de campos; ej 5_17 */	
unsigned int flags[MAXCAMPOS];	/* banderas para las distintas opciones
									bit 0: -n numeric
									bit 1: -r inverse;	ej 5_14
									bit 2: -f caps;		ej 5_15
									bit 3: -d direct;	ej 5_16 */
int (*comp[MAXCAMPOS])(void *, void *);
								/* array de MAXCAMPOS comparadores */	
#define MAXLINES 5000			/* max # de lineas por ordenar */

int readlines(char *lineptr[][MAXCAMPOS], int nlines);
void writelines(char *lineptr[][MAXCAMPOS], int nlines);
void qsort(void *s[][MAXCAMPOS], int left, int right, int (*comp[])(void *, void *));
int numcmp(char *, char *);
int strcmp_fd(char *, char *);

int campos = 0;					/* 'campos' = # de campos - 1 */
int actual;						/* 'actual' = campo actual - 1; ej 5_17 */

/* ordena lineas de entrada */
main(int argc, char *argv[])
{
	char *lineptr[MAXLINES][MAXCAMPOS];	/* matriz de apuntadores a campos */
	int nlines;							/* numero de lineas de entrada leidas */
	int c, error = NO, opciones = NO;

	flags[0] = 0;

	while(--argc > 0){
		++argv;
		while((c = *(*argv)++) && !error)
			if(opciones)				
				switch(c){
				case 'n':
					flags[campos] |= 1;
					break;
				case 'r':		/* ej 5_14 */
					flags[campos] |= 2;
					break;
				case 'f':		/* ej 5_15 */
					flags[campos] |= 4;		 
					break;
				case 'd':		/* ej 5_16 */
					flags[campos] |= 8;
					break;
				case '&':		/* ej 5_17 */
					if(flags[campos]){
						flags[++campos] = 0;
						opciones = NO;
						break;
					}	/* el caso -& (flags == 0) da error */
				default:
					printf("sort: opcion ilegal %c\n", c);
					error = SI;
					break;
				}
			else
				switch(c){
				case '-':
					opciones = SI;
					break;
				case '&':
					flags[++campos] = 0;
					break;
				default:
					error = SI;
					break;
				}
	}

	if(error){
		printf("sintaxis: sort -n -r -f -d [campo 1] &...& -n -r -f -d [campo n]\n");
		printf("entradas: texto 1 [campo1] -tab-> ... -tab-> texto n [campo n]\n");
		return -1;
	}

	for(c = 0; c <= campos; c++)
		comp[c] = (int (*)(void *, void *))(flags[c] & 1 ? numcmp : flags[c] & 12 ? strcmp_fd : strcmp);

	if((nlines = readlines(lineptr, MAXLINES)) >= 0){
		putchar('\n');
		qsort((void *(*)[MAXCAMPOS])lineptr, 0, nlines-1, comp);
		writelines(lineptr, nlines);
		return 0;
	}
	else{
		printf("entrada demasiado grande para ser ordenada\n");
		return -1;
	}
}

/* getline: trae linea y la pone en s, regresa su longitud
	version sin '\n' final */
int getline(char s[], int lim)
{
	int c, i;

	i = 0;
	while(--lim > 0 && (c=getchar()) != EOF && c != '\n')
		s[i++] = c;
	s[i] = '\0';
	return i;
}

#define MAXLEN 1000		/* max longitud de cualquier linea de entrada */

int getline(char line[], int maxline);
char *alloc(int);

/* readlines: lee lineas de entrada */
int readlines(char *lineptr[][MAXCAMPOS], int maxlines)
{
	int i, nlines, len_line;
	char *p, *pline, c, line[MAXLEN];

	nlines = 0;

	for(;;){
		len_line = getline(line, MAXLEN);
		pline = line;
		if(!len_line)
			break;
		if(nlines >= maxlines || (p = alloc(len_line+1)) == NULL)
			return -1;
		for(i = 0; i <= campos; i++){	/* encuentra los tabuladores */
				lineptr[nlines][i] = p;
				while((c = *p= *pline++) && c != '\t')
					p++;		
				if(c)	/* c == '\t' */			
					*p++ = '\0';
				else{
					while(i <= campos)		/* rellena los campos omitidos de '\0' */
						lineptr[nlines][++i] = p;
					break;
				}
		}
		nlines++;
	}
	return nlines;
}

/* writelines: escribe lineas de salida */
void writelines(char *lineptr[][MAXCAMPOS], int nlines)
{
	int i, j;

	for(i = 0; i < nlines; i++){
		for(j = 0; j <= campos; j++)
			printf("%s%s", lineptr[i][j], j < campos ? "\t" : "");
		putchar('\n');
	}
}

#define ALLOCSIZE 50000	/* tamano del espacio disponible */

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

/* qsort: clasifica v[left]...v[right] en orden ascendente */
void qsort(void *v[][MAXCAMPOS], int left, int right, int (*comp[])(void *, void *))
{
	int i, result, last;
	void swap(void *v[][MAXCAMPOS], int, int);

	if(left >= right)	/* no hace nada si el arreglo contiene */
		return;			/* menos de dos elementos */
	swap(v, left, (left + right)/2);
	last = left;
	for(i = left + 1; i <= right; i++)
		for(actual = 0; actual <= campos; actual++){
			result = (flags[actual] & 2 ? -1 : 1) * (*comp[actual])(v[i][actual], v[left][actual]);	/* ej 5_14 */
			if(!result)	/* comparacion nula -> pasamos al siguiente campo; ej 5_17 */
				continue;
			else if(result < 0)
				swap(v, ++last, i);
			break;
		}
	swap(v, left, last);
	qsort(v, left, last - 1, comp);
	qsort(v, last + 1, right,comp);
}

void swap(void *v[][MAXCAMPOS], int i, int j)
{
	void *temp;
	int k;

	for(k = 0; k <= campos; k++){
		temp = v[i][k];
		v[i][k] = v[j][k];
		v[j][k] = temp;
	}
}

#include <math.h>

/* numcmp: compara s1 y s2 numericamente */
int numcmp(char *s1, char *s2)
{
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if(v1 < v2)
		return -1;
	else if(v1 > v2)
		return 1;
	else
		return 0;
}

#include <ctype.h>

/* strcmp_fd: version de 'strcmp'
	-f sin diferenciar mayusculas de minusculas; ej 5_15
	-d solo toma letras, numeros y blancos; ej 5_16 */
int strcmp_fd(char *s, char *t)
{
	int x;

	for(;;){
		if(flags[actual] & 8){
			while(*s && !isalnum(*s) && !isspace(*s))
				s++;
			while(*t && !isalnum(*t) && !isspace(*t))
				t++;
		}
		if(!(x = (flags[actual] & 4 ? toupper(*s) - toupper(*t) : *s - *t))){
			if(!*s)
				return 0;
			s++, t++;
		}
		else
			return x;
	}
}