/* ejercicio 1_12 */

#include <stdio.h>

#define IN 1	/* en una palabra */
#define OUT 0	/* fuera de una palabra */

/* entrada -> salida; una palabra por linea */
main()
{
	int c, state;

	state = OUT;

	while((c = getchar()) != EOF){
		if(c != ' ' && c != '\n' && c != '\t'){
			state = IN;
			putchar(c);
		}
		else if(state == IN){
			putchar('\n');
			state = OUT;
		}
	}
}