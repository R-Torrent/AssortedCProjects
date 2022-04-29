/* ejercicio 2_8 */

/* rightrot(x,n): regresa el entero x rotado n posiciones 
	a la derecha; mantiene el signo */
int rightrot(int x, int n)
{
	return (~(~0<<n)&x)<<(size()-1-n) | ~0<<(size()-1)&x | ~(~0<<(size()-n-1))&x>>n;
}
/* movemos los n 1os bits tras el bit de signo; mantenemos
	el bit de signo; corremos n a la derecha */

/* size: regresa el tamaño, en bits, del tipo int */
int size()
{
	int n, x;

	for (x=n=0; (x | 01<<n) > 0; ++n)
		;

	return n+1;
}