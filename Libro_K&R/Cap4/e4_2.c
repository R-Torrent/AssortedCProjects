/* ejercicio 4_2 */

#include <ctype.h>

/* atof: convierte la cadena s a double */
double atof(char s[])
{
	double val, power;
	int i, sign, signexp, exp;

	for(i = 0; isspace(s[i]); i++)	/* ignora espacios blancos */
		;
	sign = (s[i] == '-') ? -1 : 1;
	if(s[i] == '+' || s[i] == '-')
		i++;
	for(val = 0.0; isdigit(s[i]); i++)
		val = 10.0 * val + (s[i] - '0');
	if(s[i] == '.')
		i++;
	for(power = 1; isdigit(s[i]); i++){
	   	val = 10.0 * val + (s[i] - '0');
		power *= 10;
	}
	if(s[i] == 'e' || s[i] == 'E')
		i++;
	signexp = (s[i] == '-') ? -1 : 1;
	if(s[i] == '+' || s[i] == '-')
		i++;
	for(exp = 0; isdigit(s[i]); i++)
		exp = 10 * exp + (s[i] - '0');
	while(exp--)
		if(signexp == -1)
			power *= 10;
		else
			power /= 10;
	return sign * val / power;
}