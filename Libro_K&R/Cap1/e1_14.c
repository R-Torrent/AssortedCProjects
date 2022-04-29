/* ejercicio 1_14 */
/* histogramas de frecuencias de caracteres */

#include <stdio.h>

#define ASCII 256	/* el codigo ascii incluye 256 caracteres */
#define TOTAL 20	/* frecuencia del 100% se representa mediante TOTAL *s */	

main()
{
	int c, i, j, numc;		/* numc: numero de caracteres */
	float codigo[ASCII];		/* ocurrencias de cada caracter ASCII */
	int histo[ASCII];			/* para la representacion de frecuencias */
	
	numc = 0;
	for(i = 0; i < ASCII; ++i)
		codigo[i] = 0;
	
	while((c = getchar()) != EOF){
		++codigo[c];	
		++numc;
	}			
	
	/* transformacion de ocurrencias en frecuencias */
	if(numc != 0)
		for(i = 0; i < ASCII; i++)
			codigo[i] = codigo[i] / numc; 

	/* valores para los campos del histograma */
	for(i = 0; i < ASCII; i++)
		histo[i] = codigo[i] * TOTAL;

	/* impresion de resultados */
	printf("Histograma de frecuencias:\n");
	for(i = 0; i < ASCII; ++i)
		if(codigo[i] != 0){		/* una frecuencia baja sin * no apareceria si */
			putchar(i);			/* pusieramos histo[i] en vez de codigo[i] */
			putchar(':');
			for(j = 0; j < TOTAL; ++j)
				if(j < histo[i])
					putchar('*');
				else
					putchar(' ');
			putchar('|');
			printf("%.3f\n", codigo[i]);
		}

	printf("\nNumero de caracteres leidos: %d\n", numc);
}