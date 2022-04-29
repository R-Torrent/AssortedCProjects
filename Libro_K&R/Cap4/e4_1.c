/* ejercicio 4_1 */

#include <string.h>

/* strrindex: regresa la ultima posicion de t en s, -1 si no existe */
int strrindex(char s[], char t[])
{
	int i, j, k;

	for(i=strlen(s)-strlen(t); i>=0; i--){
		for(j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++)
			;
		if(k>0 && t[k]=='\0')
			return i;
	}
	return -1;
}