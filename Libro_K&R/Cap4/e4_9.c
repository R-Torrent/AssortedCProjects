/* ejercicio 4_9 */
/* version 'getch' y 'ungetch' del programa e4_3_6
   manejando correctamente un EOF */

#define BUFSIZE 100

char buf[BUFSIZE];	/* buffer para ungetch */
int bufp = 0;		/* siguiente posicion libre en buf */
int eofp = BUFSIZE;	/* posicion del EOF */
			/* si eofp == BUFSIZE, no hay EOF */
	
int getch(void)	/* obtiene un (posiblemente ya regresado) caracter */
{
	if(bufp < eofp)
		return (bufp > 0) ? buf[--bufp] : getchar();
	else{
		--bufp;
		eofp = BUFSIZE;
		return EOF;
	}
}

void ungetch(int c)	/* regresa caracter a la entrada */
{
	if(bufp >= BUFSIZE)
		printf("ungetch: demasiados caracteres\n");
	else if(c == EOF)
		eofp = bufp++;
	else
		buf[bufp++] = c;
}
/* evitamos char buf[BUFSIZE] -> int buf[BUFSIZE] por ser el
   arreglo de enteros excesivo */