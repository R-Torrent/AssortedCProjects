/* ejercicio 5_10 */

#include <stdio.h>
#include <stdlib.h>	/* para atof() */

#define MAXOP 100	/* max tamano de operando u operador */
#define NUMBER '0'	/* senal que un numero se encontro */
					
int getop(char *, char []);
void push(double);
double pop(void);

/* calculadora polaca inversa */
main(int argc, char *argv[])
{
	int type;
	double op2;	
	char s[MAXOP];

	while(--argc != 0){ 
		type = getop(*++argv, s);
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
		default:
			printf("error: comando desconocido %s\n", s);
			break;
		}
	}
	printf("\t%.8g\n", pop());
		
	return 0;
}

#define MAXVAL 100	/* maxima profundidad de la pila val */

int sp = 0;			/* siguiente posicion libre en la pila */
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
int getop(char *arg, char s[])
{
	int i = 0, c, sign = 1;

	while((s[0] = c = *arg++) == '\t')
		;
	s[1] = '\0';
	if(!isdigit(c) && c != '.'){
		return c;		/* no es un numero */
	}
	i = 0;
	if(isdigit(c))		/* reune la parte entera */
		while((s[++i] = c = *arg++))
			;
	if(c == '.')		/* reune la parte fraccionaria */
		while(isdigit(s[++i] = c = *arg++))
			;
	s[i] = '\0';
	
	return NUMBER;
}