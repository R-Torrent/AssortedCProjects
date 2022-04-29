/* ejercicio 7_4 */

#include <stdio.h>
#include <stdarg.h>

#define MODIFICADORES 15 /* longitud del string de los especificadores de conversion */

/* minscanf: scanf minima con lista variable de argumentos */
void minscanf(char *fmt, ...)
{
	va_list ap; /* apunta a cada arg sin nombre en orden */
	char *p, s[MODIFICADORES];
	int i = 0;
	enum boolean{NO, SI} espec = NO;

	va_start(ap, fmt); /* hace que ap apunte al 1er. arg sin nombre */
	for(p=fmt; *p; p++){
		s[i++] = *p;
		s[i] = '\0';
		if(!espec){
			if(*p == '%')
				espec = SI;
			else{
				scanf(s);
				i = 0;
			}
			continue;
		}
		switch(*p){
		case 'd': case 'i': case 'o': case 'x':
			scanf(s, va_arg(ap, int *));
			break;
		case 'u':
			scanf(s, va_arg(ap, unsigned int *));
			break;
		case 'e': case 'f': case 'g':
			scanf(s, va_arg(ap, double *));
			break;
		case 's': case 'c':
			scanf(s, va_arg(ap, char *));
			break;
		case 'p':
			scanf(s, va_arg(ap, void *));
			break;
		case '%':
			scanf(s);
			break;
		default:
			continue;
		}
		i = 0, espec = NO;
	}
	va_end(ap); /* limpia cuando todo esta hecho */
}