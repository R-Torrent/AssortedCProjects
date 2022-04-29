/* ejercicio 4_10 */
/* version usando 'getline' modificado */

#include <stdio.h>
#include <stdlib.h>	/* para atof() */

#define MAXOP 100	/* max tamano de operando u operador */
#define NUMBER '0'	/* senal que un numero se encontro */
#define MAXLINE 1000	/* longitud maxima por linea de entrada */
					
int getop(char []);
void push(double);
double pop(void);

void getline(char [], int);
char line[MAXLINE];
int linep = 0;

/* calculadora polaca inversa */
main()
{
	int type;
	double op2;	
	char s[MAXOP];

	getline(line, MAXLINE);

	while((type = getop(s)) != EOF){
		switch(type){
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if(op2 != 0.0)
				push(pop() / op2);
			else
				printf("error: divisor cero\n");
			break;
		case '\n':
			printf("\t%.8g\n", pop());
			getline(line, MAXLINE);
			break;
		default:
			printf("error: comando desconocido %s\n", s);
			break;
		}
	}
	return 0;
}

#define MAXVAL 100	/* maxima profundidad de la pila val */

int sp = 0;		/* siguiente posicion libre en la pila */
double val[MAXVAL];	/* valores de la pila */

/* push: introduce f a la pila */
void push(double f)
{
	if(sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: pila llena, no se puede efectuar push %g\n", f);
}

/* pop: extrae y regresa el valor superior de la pila */
double pop(void)
{
	if(sp > 0)
		return val[--sp];
	else{
		printf("error: pila vacia\n");
		return 0.0;
	}
}

#include <ctype.h>

/* getop: obtiene el siguiente operador u operando numerico */
int getop(char s[])
{
	int i, c;

	while((s[0] = c = line[linep++]) == ' ' || c == '\t')
		;
	s[1] = '\0';
	if(!isdigit(c) && c != '.')
		return c;		/* no es un numero */
	i = 0;
	if(isdigit(c))		/* reune la parte entera */
		while(isdigit(s[++i] = c = line[linep++]))
			;
	if(c == '.')		/* reune la parte fraccionaria */
		while(isdigit(s[++i] = c = line[linep++]))
			;
	s[i] = '\0';
	if(c != EOF)
		--linep;
	return NUMBER;
}

/* getline: trae linea y la pone en s (incluye EOF) */
void getline(char s[], int lim)
{
	int c, i;
	
	i = linep = 0;
	while(--lim > 0 && (c=getchar()) != EOF && c != '\n')
		s[i++] = c;
	if(c == '\n' || c == EOF)
		s[i++] = c;
	s[i] = '\0';
}