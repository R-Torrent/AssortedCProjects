/* ejercicio 1_9 */
/* entrada -> salida; reemplaza una cadena de >= 1 blancos en uno */

#include <stdio.h>

main()
{
	int c;
	
	while((c = getchar()) != EOF){
		putchar(c);
		if(c == ' '){
			while((c = getchar()) == ' ')
				;
			putchar(c);
		}
	}
}