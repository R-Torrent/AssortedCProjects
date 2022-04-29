/* ejercicio 7_5 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>	/* ej 4_5 */
#include <ctype.h>

#define MAXLINE 1000	/* tamano maximo de linea */
#define MAXOPSLINE 1000	/* max numero de operadores por linea */
#define NUMBER '0'	/* senal de que un numero se encontro */
#define ALPHA 'A'	/* ej 4_6; senal de que una variable se encontro
					   variables: $a -> $z [26]
						$z == valor impreso mas reciente
					   uso:
						$x == push(mem(x))
						$$x == mem(x)=push(pop()) */

struct ops {
	int type;
	char *s;
};

int getline(char [], int);
int getops(char [], struct ops [], int);
void push(double);
double pop(void);

/* calculadora polaca inversa */
main()
{
	int nops;
	extern int sp;	/* ej 4_4; opcion 'x' */
	double op;	/* ej 4_4 */
	double op2;	
	static double mem[26];	/* ej 4_6; vector memorias (inicialmente = 0.0) */
	char line[MAXLINE];
	struct ops lineops[MAXOPSLINE], *plineops; /* todos los operadores de la linea */

	while(getline(line, MAXLINE)){
		for(nops = getops(line, lineops, MAXOPSLINE), plineops = lineops; nops--; plineops++)
			switch(plineops->type){
				case NUMBER:
					push(atof(plineops->s));
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
					printf("error: comando desconocido %s\n", plineops->s);
					break;
				/* ej 4_3 */
				case '%':	
					op2 = pop();
					if((int)op2 != 0)
						push((int)pop() % (int)op2);
					else
						printf("error: divisor cero\n");
					break;
				/* ej 4_4 */
				case 'v':	/* imprimir */
					push(op = pop());
					printf("\nver: %.8g\n", op);
					break;
				case 'd':	/* duplicar */
					push(op=pop());
					push(op);
					break;
				case 'w':	/* intercambiar */
					op = pop();
					op2 = pop();
					push(op);
					push(op2);
					break;
				case 'x':  /* limpiar pila */
					sp = 0;			
					break;
				/* ej 4_5 */
				case 's':
					push(sin(pop()));
					break;
				case 'e':
					push(exp(pop()));
					break;
				case 'p':
					op2 = pop();
					push(pow(pop(), op2));
					break;
				/* ej 4_6 */
				case ALPHA:	/* solo ASCII, llega con un $ de menos */
					if(lineops->s) /* control de error */
						if(*plineops->s == '$')
							push(mem[tolower(*(plineops->s+1)) - 'a'] = pop());
						else
							push(mem[tolower(*plineops->s) - 'a']);
				break;
			}
		if(sp){ /* fin de linea */
			push(mem[25] = pop());	/* ej 4_6 */
			printf("Resultado [->$z]: %.8g\n",mem[25]);	/* mem[25]==$z */
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
	if(sp < (MAXVAL-1))
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

/* getops: obtiene todos los operadores u operandos numericos de una linea */
int getops(char line[], struct ops *op, int lim)
{
	int i;
	float f;
	char c;

	for(i = 0; lim-- && (op->s = strtok(line, " \t\n")); line = NULL, op++, i++)
		if(sscanf(op->s, "%f", &f) == 1)
			op->type = NUMBER;
		else if((c = *op->s) == '$'){	/* ej 4_6 */
			if((c = *++op->s) == '$') /* eliminamos el primer $ */
				c = *(op->s+1);
			if(!isalpha((int)c)){
				op->s = NULL;	/* senal de error */
				printf("error: variable incorrecta %c\n",c);
			}
			op->type = ALPHA;
		}
		else
			op->type = (c == ALPHA) ? 0 : (int)c; /* no es un numero ni variable */
	return i;
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
	if(c == EOF)
		putchar(c);
	s[i] = '\0';
	return i;
}