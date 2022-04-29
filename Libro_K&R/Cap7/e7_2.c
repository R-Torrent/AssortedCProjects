/* ejercicio 7_2 */

#include <stdio.h>
#include <ctype.h>

#define MAXLINE 80

int main()
{
	int c, size, i = 1;
	char caracter[5];
	
	while((c = getchar()) != EOF){
		size = sprintf(caracter, isprint(c) ? "%c" : "0x%x", c);
		if(MAXLINE - i >= size){
			printf("%s", caracter);
			i += size;
		}
		else{
			printf("\n%s", caracter);
			i = size + 1;
		}
	}
	return 0;
}