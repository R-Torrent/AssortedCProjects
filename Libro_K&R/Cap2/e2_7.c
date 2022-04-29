/* ejercicio 2_7 */

/* invert(x,p,n): regresa x con los n bits que principian en
	la posicion p invertidos (1->0, 0->1)
	(posicion 1er bit [extremo derecho]: bit 0) */
unsigned invert(unsigned x, int p, int n)
{
	return (x&~(~(~0<<n)<<p)) | (~x&(~(~0<<n)<<p));
}
/* aplicamos a x con & una mascara de 1s con
	n 0s desde p; aplicamos a ~x con & una mascara de 0s con
	n 1s desde p; copiamos con | */ 					
