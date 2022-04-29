/* ejercicio 2_9 */

/* bitcount: cuenta bits 1 en x */
int bitcount(unsigned x)
{
	int b=0;

	while(x){
		x&=x-1;
		++b;
	}

	return b;
}
/* x & x-1 elimina el bit menos significativo de x */
