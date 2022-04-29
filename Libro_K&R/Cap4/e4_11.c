/* ejercicio 4_11 */
/* version 'getop' y 'getch' del programa e4_3_6
   con una variable tipo 'static';
   prescinde de 'ungetch' */

/* getop: obtiene el siguiente operador u operando numerico */
int getop(char s[])
{
	int i, c; 
	static int c1 = 0;
	/* la inicializacion de variables 'static'
		es de la seccion 4.9...
	   c1 == 0 -> c1 vacio */
	
	if(c1)
		c = c1, c1 = 0;	/* borramos c1 */
	else
		c = getch();
	while((s[0] = c) == ' ' || c == '\t')
		c = getch();
	s[1] = '\0';
	if(!isdigit(c) && c != '.')
		return c;	/* no es un numero */
	i = 0;
	if(isdigit(c))		/* reune la parte entera */
		while(isdigit(s[++i] = c = getch()))
			;
	if(c == '.')		/* reune la parte fraccionaria */
		while(isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	c1 = c;	/* almacenamos en c1 */
	return NUMBER;
}

int getch(void)	/* obtiene un (posiblemente ya regresado) caracter */
{
	return getchar();
}