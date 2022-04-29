/* ejercicio 1_4 */
#include <stdio.h>
/* imprime la tabla Celsius-Fahrenheit
	para celsius = -40, -20, ..., 100; version de punto flotante
	con encabezado */
main()
{
	float fahr, celsius;
	int lower, upper, step;
	
	lower = -40;	/* limite inferior de la tabla de temperaturas */
	upper = 100;	/* limite superior */
	step = 20;		/* tamaño del incremento */
	
	celsius = lower;
	printf("Celsius Fahren.\n");
	printf("---------------\n");
	while(celsius <= upper){
		fahr = (9.0/5.0) * celsius + 32.0;
		printf("%7.0f %7.1f\n", celsius, fahr);
		celsius = celsius + step;
	}
} 