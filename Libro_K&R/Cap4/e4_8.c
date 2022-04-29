/* ejercicio 4_8 */
/* version 'getch' y 'ungetch' del programa e4_3_6
   con un buffer de 1 caracter */

char buf;		/* buffer para ungetch */
int condbuf = 0;	/* buffer vacio: 0, lleno: 1 */

int getch(void)	/* obtiene un (posiblemente ya regresado) caracter */
{
	if(condbuf){
		condbuf = 0;
		return buf;
	}
	else
		return getchar();
}

void ungetch(int c)	/* regresa caracter a la entrada */
{
	if(condbuf)
		printf("ungetch: demasiados caracteres\n");
	else{
		buf = c;
		condbuf = 1;
	}
}