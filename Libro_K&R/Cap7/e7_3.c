/* ejercicio 7_3 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MODIFICADORES 15 /* longitud del string de los especificadores de conversion */

int itoa(int, char []);
void reverse(char []);

/* minprintf: printf minima con lista variable de argumentos */
void minprintf(char *fmt, ...)
{
	va_list ap; /* apunta a cada arg sin nombre en orden */
	char *p, s[MODIFICADORES];
	int i = 0;
	enum boolean{NO, SI} espec = NO;

	va_start(ap, fmt); /* hace que ap apunte al 1er. arg sin nombre */
	for(p=fmt; *p; p++){
		if(!espec){
			if(*p != '%')
				putchar(*p);
			else{
				s[i++] = '%';
				espec = SI;
			continue;
		}
		s[i++] = *p;
		s[i] = '\0';
		switch(*p){
		case 'd': case 'i': case 'o': case 'x': case 'X':
			printf(s, va_arg(ap, int));
			break;
		case 'u':
			printf(s, va_arg(ap, unsigned int));
			break;
		case 'e': case 'E': case 'f': case 'g': case 'G':
			printf(s, va_arg(ap, double));
			break;
		case 's': case 'c':
			printf(s, va_arg(ap, char *));
			break;
		case 'p':
			printf(s, va_arg(ap, void *));
			break;
		case '%':
			printf(s);
			break;
		case '*':
			i += itoa(va_arg(ap, int), &s[--i]); /* borra el caracter '*' */
		default:
			continue;
		}
		i = 0, espec = NO;
	}
	va_end(ap); /* limpia cuando todo esta hecho */
}

/* itoa: convierte n a caracteres en s */
int itoa(int n, char s[])
{
	int i = 0, sign;

	if((sign = n) < 0)	/* registra el signo */
		n = -n;	/* vuelve a n positivo */
	
	do{		/* genera digitos en orden inverso */
		s[i++] = n % 10 + '0';	/* toma el siguiente digito */	
	}while((n /= 10) > 0);		/* borralo */
	if(sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
	return i;
}

/* reverse: invierte parte de la cadena s en el mismo lugar */
void reverse(char s[])
{
	int c, i, j;

	for(i = 0, j = strlen(s)-1; i < j; i++, j--){
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}