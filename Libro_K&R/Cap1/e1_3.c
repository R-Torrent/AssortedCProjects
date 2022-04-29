/* ejercicio 1_3 */

#include <stdio.h>
/* imprime la tabla Fahrenheit-Celsius
	para fahr = 0, 20, ...,300; version de punto flotante
	con encabezado */
main()
{
	float fahr, celsius;
	int lower, upper, step;
	
	lower = 0;		/* limite inferior de la tabla de temperaturas*/
	upper = 300;	/* limite superior */
	step = 20;		/* tamaño del incremento */
	
	fahr = lower;
	printf("Fahren. Celsius\n");
	printf("---------------\n");
	while(fahr <= upper){
		celsius = (5.0/9.0) * (fahr-32.0);
		printf("%7.0f %7.1f\n", fahr, celsius);
		fahr = fahr + step;
	}
} 