/* ejercicio 1_8 */
/* cuenta espacios en blanco, tabuladores y nuevas lineas */

#include <stdio.h>

main()
{
	int c, nb, nt, nl;

	nb = 0;
	nt = 0;
	nl = 0;

	while((c = getchar()) != EOF){
		if(c == ' ')
			++nb;
		if(c == '\t')
			++nt;
		if(c == '\n')
			++nl;
	}

	printf("\nNumero de espacios en blanco: %d", nb);
	printf("\nNumero de tabuladores: %d", nt);
	printf("\nNumero de nuevas lineas: %d", nl);
}
