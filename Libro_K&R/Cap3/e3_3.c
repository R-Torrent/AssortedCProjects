/* ejercicio 3_3 */

#include <ctype.h>

/* expand: expande notacion abreviada como a-z en s1,
	en el equivalente abc...xyz en s2; solo ASCII */
void expand(char s1[], char s2[])
{
	int i, j;

	for(i=j=0; (s2[j]=s1[i]) != '\0'; ++i, ++j)
		if(s1[i]=='-' && i>0 && ((islower(s1[i-1]) && islower(s1[i+1])) || (isupper(s1[i-1]) && isupper(s1[i+1])) || (isdigit(s1[i-1]) && isdigit(s1[i+1]))) && s1[i+1]>s1[i-1]){
			while((s2[j]=s2[j-1]+1) != s1[i+1])
				++j;
			++i;
		}
}