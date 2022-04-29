/* ejercicio 5_2 */

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getfloat: obtiene el siguiente entero de la entrada y lo asigna a *pn */
int getfloat(double *pn)	/* getfloat devuelve un entero */
{
	int c, sign;
	double power = 1;

	while(isspace(c = getch()))	/* ignora espacios en blanco */
		;
	if(!isdigit(c) && c != EOF && c != '+' && c != '-' && c != '.')
		return 0;		/* no es un numero */
	sign = (c == '-')?-1:1;
	if(c == '+' || c == '-')
		if(!isdigit(c = getch()) && c != '.'){
			ungetch(c);
			return 0;
		}
	*pn = 0.0;
	if(isdigit(c))		/* reune la parte entera */
		do{
			*pn = 10 * *pn + (c - '0');
		}while(isdigit(c = getch()));
	if(c == '.')		/* reune la parte fraccionaria */
		while(isdigit(c = getch())){
			*pn = 10 * *pn + (c - '0');
			power *= 10;
		}
	*pn *= sign / power;
	if(c != EOF)
		ungetch(c);
	return c;
}