/* ejercicio 4_12 */

/* itoa: convierte n a caracteres en s;
   version recursiva */
void itoa(int n, char s[])
{
	static int i, sign;

	if(n < 0){
		sign = n;	/* registra el signo */
		n = -n;		/* vuelve a n positivo */
	}
	i = 0;
	if(n/10)
		itoa(n/10, s);
	if(sign < 0){
		s[i++] = '-';
		sign = -sign;
	}
	s[i++] = n % 10 + '0';
	s[i] = '\0';
}