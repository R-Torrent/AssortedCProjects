/* ejercicio 5_6 */
/* versiones con apuntadores de:
	'getline' (sec 4.1)
	'atoi' (sec 3.5)
	'itoa' (sec 3.6)
	'reverse' (sec 3.5)
	'strindex' (sec 4.1)
	'getop' (sec 4.3) */

#include <stdio.h>	/* para 'getline' y 'getch' */
#include <ctype.h>	/* para 'atoi' y 'getop' */
#include <string.h>	/* para 'reverse' */

/* getline: trae linea y la pone en s, regresa su longitud */
int getline(char *s, int lim)
{
	int c;
	char *s1 = s;

	while(--lim > 0 && (c=getchar()) != EOF && c != '\n')
		*s++ = c;
	if(c == '\n')
		*s++ = c;
	*s = '\0';
	return s-s1;
}

/* atoi: convierte s a entero */
int atoi(char *s)
{
	int n, sign;

	while(isspace(*s))	/* ignora espacios en blanco */
		s++;
	sign = (*s == '-') ? -1 : 1;
	if(*s == '+' || *s == '-')	/* ignora el signo */
		s++;
	for(n = 0; isdigit(*s); s++)
		n = 10 * n + (*s - '0');
	return sign * n;
}

/* itoa: convierte n a caracteres en s */
void itoa(int n, char *s)
{
	int sign;
	char *s1 = s;

	if((sign = n) < 0)	/* registra el signo */
		n = -n;			/* vuelve a n positivo */
	do{			/* genera digitos en orden inverso */
		*s++ = n % 10 + '0';	/* toma el sigiente digito */
	}while((n /= 10) > 0);		/* borralo */
	if(sign < 0)
		*s++ = '-';
	*s = '\0';
	reverse(s1);
}

/* reverse: invierte la cadena s en el mismo lugar */
void reverse(char *s)
{
	char c;
	char *t = s + strlen(s) - 1;

	for(; s < t; s++, t--){
		c = *s;
		*s = *t;
		*t = c;
	}
}

/* strindex: regresa el indice de t en s, -1 si no existe */
int strindex(char *s, char *t)
{
	char *s1, *s2, *t1;

	for(s1 = s; *s1; s1++){
		for(s2 = s1, t1 = t; *t1 && *s2 == *t1; s2++, t1++)
			;
		if(t1 - t > 0 && !*t1)
			return s1 - s;
	}
	return -1;
}

#define NUMBER '0'	/* senal de que un numero se encontro */

int getch(void);
void ungetch(int);

/* getop: obtiene el siguiente operador u operando numerico */
int getop(char *s)
{
	int c;

	while((*s = c = getch()) == ' ' || c == '\t')
		;
	*(s+1) = '\0';
	if(!isdigit(c) && c != '.')
		return c;		/* no es un numero */
	if(isdigit(c))		/* reune la parte entera */
		while(isdigit(*++s = c = getch()))
			;
	if(c == '.')	/* reune la parte fraccionaria */
		while(isdigit(*++s = c = getch()))
			;
	*s = '\0';
	if(c != EOF)
		ungetch(c);
	return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE];	/* buffer para ungetch */
char *bufp = buf;	/* siguiente posicion libre en buf */

int getch(void)	/* obtiene un (posiblemente ya regresado) caracter */
{
	return (bufp > buf) ? *--bufp : getchar();
}

void ungetch(int c)	/* regresa caracter a la entrada */
{
	if(bufp - buf >= BUFSIZE)
		printf("ungetch: demasiados caracteres\n");
	else
		*bufp++ = c;
}