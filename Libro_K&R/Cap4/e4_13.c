/* ejercicio 4_13 */

#include <string.h>

/* reverse: invierte la cadena s en el mismo lugar;
   version recursiva */
void reverse(char s[])
{
	static int i = 0, len;
	int c, j;
	
	/* por rapidez, llamamos a strlen solo 1 vez */
	if(!i)
		len = strlen(s);
	j = len - 1 - i;

	c = s[i], s[i] = s[j], s[j] = c; /* swap */
	if(i < j-1){
		++i;
		reverse(s);
		--i;
	}
}