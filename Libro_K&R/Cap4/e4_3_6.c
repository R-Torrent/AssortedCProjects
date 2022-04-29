/* ejercicios 4_3 hasta 4_6 */

#include <stdio.h>
#include <stdlib.h>	/* para atof() */
#include <math.h>	/* ej 4_5 */

#define MAXOP 100	/* max tamano de operando u operador */
#define NUMBER '0'	/* senal que un numero se encontro */
#define ALPHA 'A'	/* ej 4_6; senal que una variable se encontro */
					
int getop(char []);
void push(double);
double pop(void);

/* calculadora polaca inversa */
main()
{
	int type;
	extern int sp;	/* ej 4_4; opcion 'x' */
	double op;	/* ej 4_4 */
	double op2;	
	double mem[26];	/* ej 4_6; vector memorias */
	char s[MAXOP];

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
			printf("\t%.8g\n", mem[25] = pop()); /* ej 4_6 */
			break;								 /* mem[25]==$z */
		default:
			printf("error: comando desconocido %s\n", s);
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
			break;	/* duplicar */
		case 'd':
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
		case ALPHA:	/* solo ASCII */
			/* variables: $a -> $z [26]
				$z == valor impreso mas reciente
			   uso:
				$x == push(mem(x))
				$$x == mem(x)=push(pop()) */
			if(s[0] != 'E')
				if(s[0] == '$')
					push(mem[s[1]-'a'] = pop());
				else
					push(mem[s[0]-'a']);
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

int getch(void);
void ungetch(int);

/* getop: obtiene el siguiente operador u operando numerico */
int getop(char s[])
{
	int i = 0, c, sign = 1;

	while((s[0] = c = getch()) == ' ' || c == '\t')
		;
	if(c == '-'){		/* ej 4_3 */
		sign = -1;
		s[i = 1] = c = getch();
	}
	if(!isdigit(c) && c != '.'){
		if(sign == -1){	/* ej 4_3 */
			if(c != EOF)
				ungetch(c);
			return '-';	/* operador '-' */
		}
		if(c ==	'$'){	/* ej 4_6 */
			s[i++] = tolower(c = getch());	/* $a == $A */
			if(c == '$')
				s[i++] = c = getch();
			if(!islower(c)){
				s[0] = 'E';	/* senal de error */
				if(c != EOF)
					ungetch(c);
				printf("error: variable sin nombre\n");
			}
			s[i] = '\0';
			return ALPHA;
		}
		return c;		/* no es un numero */
	}
	if(isdigit(c))		/* reune la parte entera */
		while(isdigit(s[++i] = c = getch()))
			;
	if(c == '.')		/* reune la parte fraccionaria */
		while(isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if(c != EOF)
		ungetch(c);
	return NUMBER;
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