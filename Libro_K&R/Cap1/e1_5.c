/* ejercicio 1_5 */

#include <stdio.h>
/* imprime la tabla Fahrenheit-Celsius 
en orden inverso: 300, 280, ..., 0 */
main()
{
	int fahr;
	
	for (fahr = 300; fahr >= 0; fahr = fahr - 20)
		printf("%3d %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
} 
