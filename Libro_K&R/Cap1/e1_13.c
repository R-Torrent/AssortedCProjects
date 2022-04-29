/* ejercicio 1_13 */
/* histogramas de longitudes de las palabras de entrada */

#include <stdio.h>

#define SI 1	/* en una palabra/barras verticales */
#define NO 0	/* fuera de una palabra/barras horizontales */
#define MAX 10	/* campos: 1, 2, ..., >=MAX */

main()
{
	int vh, state, c, i, j, longitud, numpal, longmax;
			/* longmax sirve para conocer la altura max
			   en orientacion vertical */
	int longpal[MAX];		/* longitudes de 1 -> >=MAX caracters */

	numpal = longitud = longmax = 0;
	state = NO;

	for(i = 0; i < MAX; ++i)
		longpal[i] = 0;
	
	printf("Histograma horizontal (h) o vertical (v)? [h por defecto] ");
	vh = getchar();
	if(vh == 'v')
		vh = SI;
	else
		vh = NO;

	while((c = getchar()) != EOF){
		if(c != ' ' && c != '\n' && c != '\t'){
			++longitud;
			if(state == NO){
				++numpal;
				state = SI;
			}
		}
		else if(state == SI){
			if (longitud > MAX)
					longitud = MAX;
			++longpal[longitud-1];
			longitud = 0;
			state = NO;
		}
	}				

	/* conocer longmax */
	for(i = 0; i < MAX; ++i)
		if(longpal[i] > longmax)
			longmax = longpal[i];
	
	/* impresion de resultados */
	printf("Numero de palabras segun longitud:\n\n");
	if(vh == NO)	/* horizontal */
		for(i = 0; i < MAX; ++i){
			printf("\n%2d: ", i + 1);
			for(j = 0; j < longpal[i]; ++j)
				putchar('*');
		}
	else{	/* vertical */
		for(j = longmax; j > 0; --j){
			for(i = 0; i < MAX; ++i)
				if(longpal[i] >= j)
					printf(" * ");
				else
					printf("   ");
			putchar('\n');
		}
			
		for(i = 0; i < MAX; ++i)
			printf("---");
		putchar('\n');
		for(i = 0; i < MAX; ++i)
			printf("%3d", i + 1);
	}

	printf("\n\nNumero de palabras: %d\n", numpal);
}