/* ejercicio 1_15 */
/* conversion de temperaturas usando funciones */

#include <stdio.h>
/* imprime la tabla Fahrenheit-Celsius
	para fahr = 0, 20, ..., 300 */

int celsius(int fahrenheit);

main()
{
	int fahr;
	int lower, upper, step;

	lower = 0;		/* limite inferior de la tabla de temperaturas */
	upper = 300;	/* limite superior */
	step = 20;		/* tamaño del incremento */

	fahr = lower;
	while(fahr <= upper){
		printf("%d\t%d\n", fahr, celsius(fahr));
		fahr = fahr + step;
	}
return 0;
}

int celsius(int temp)
{
	return 5 * (temp-32) / 9;
}