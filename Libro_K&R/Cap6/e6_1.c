/* ejercicio 6_1 */
/* version mejorada de 'getword' */

#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof keytab[0])

struct key{
	char *word;
	int count;
} keytab[] = {
	"auto", 0,
	"break", 0,
	"case", 0,
	"char", 0,
	"const", 0,
	"continue", 0,
	"default", 0,
	"do", 0,
	"double", 0,
	"else", 0,
	"enum", 0,
	"extern", 0,
	"float", 0,
	"for", 0,
	"goto", 0,
	"if", 0,
	"int", 0,
	"long", 0,
	"register", 0,
	"return", 0,
	"short", 0,
	"signed", 0,
	"sizeof", 0,
	"static", 0,
	"struct", 0,
	"switch", 0,
	"typedef", 0,
	"union", 0,
	"unsigned", 0,
	"void", 0,
	"volatile", 0,
	"while", 0
};

/* comprueba, comentarios, cadenas constantes y
	el preprocesador */
enum boolean{
	NO,
	SI,
} comentario = NO, cadena = NO, procesador = NO, extension = NO;
/* extension: extension del preprocesador a mas lineas */

int getword(char *, int);
int binsearch(char *, struct key *, int);

/* cuenta palabras reservadas de C */
main()
{
	int n, current, last = 0, b_last;
	char word[MAXWORD];

	while((current = getword(word, MAXWORD)) != EOF){
		if(cadena){
			if(current == '\"')
				cadena = NO;
		}
		else if(comentario){
			if(current == '/' && last == '*')
				comentario = NO;
		}
		else if(isalpha(word[0]) && !procesador){
			if((n = binsearch(word, keytab, NKEYS)) >= 0)
				keytab[n].count++;
		}
		else{
			if(current == '*' && last == '/'){
				comentario = SI;
				if(extension && b_last == '/')
					extension == NO;
			}
			else if(procesador){
				if(current == '\n' && !extension)
					procesador = NO;
				else if(current == '\\')
					extension = SI;
				else if(current != '/')
					extension = NO;
			}
			else if(current == '#' && last == '\n')
				procesador = SI;
			else if(current == '\"')
				cadena = SI;
		}
		b_last = last;
		last = current;
	}
	putchar('\n');
	for(n = 0; n < NKEYS; n++)
		if(keytab[n].count > 0)
			printf("%4d %s\n", keytab[n].count, keytab[n].word);

	return 0;
}

/* binsearch: encuentra una palabra en tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;
	while(low <= high){
		mid = (low + high) / 2;
		if((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if(cond > 0)
			low = mid + 1;
		else return mid;
	}
	return -1;
}

/* getword: obtiene la siguiente palabra o caracter de la entrada */
int getword(char *word, int lim)
{
	int c, getch(void);
	void ungetch(int);
	char *w = word;

	while(isspace(c = getch()))
		;
	if( c!= EOF)
		*w++ = c;
	if(!isalpha(c)){
		*w = '\0';
		return c;
	}
	for(; --lim > 0; w++)
		if(!isalnum(*w = getch())){
			ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
}

#define BUFSIZE 100

char buf[BUFSIZE];	/* buffer para ungetch */
int bufp = 0;		/* siguiente posicion libre en buf */

int getch(void)	/* obtiene un (posiblemente ya regresado) caracter */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)	/* regresa caracter a la entrada */
{
	if(bufp >= BUFSIZE)
		printf("ungetch: demasiados caracteres\n");
	else
		buf[bufp++] = c;
}