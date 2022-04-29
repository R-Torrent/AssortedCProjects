/* ejercicio 1_10 */
/* entrada -> salida; reemplazando:
	tabulacion por \t
	retroceso por \b
	diagonal invertida por \\ */

#include <stdio.h>

main()
{
	int c;

	while((c = getchar()) != EOF){
		if(c != '\t')
			if(c != '\b')
				if(c != '\\')
					putchar(c);
		if(c == '\t')
			printf("\\t");
		if(c == '\b')
			printf("\\b");
		if(c == '\\')
			printf("\\\\");
	}
}
