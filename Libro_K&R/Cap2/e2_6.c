/* ejercicio 2_6 */

/* setbits(x,p,n,y): regresa x con los n bits que principian
	en la posicion p iguales a los n bits mas a la
	derecha de y, dejando los otros bits sin cambio 
	(posicion 1er bit [extremo derecho]: bit 0) */
unsigned setbits(unsigned x, int p, int n, unsigned y)
{
	return (x&~(~(~0<<n)<<p)) | ((y&~(~0<<n))<<p);
}
/* aplicamos a x con & una mascara de 1s con n 0s desde p;
	cogemos los primeros n de y y los
	desplazamos hasta p; copiamos a x con | */
					
