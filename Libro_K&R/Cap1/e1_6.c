/* ejercicio 1_6 */
/* verificacion que getchar() != EOF es 0 o 1 */

#include <stdio.h>

main()
{
	int i;
	
	printf("Escriba hasta 10 caracteres.\n");
	printf("El resultado es getchar() != EOF.\n");

	for(i=0; i<10; i=i+1)
		printf("%d", getchar() != EOF);

	printf("\n");

}