/* ejercicio 3_4 */

#include <string.h>

/* itoa: convierte n a caracteres en s */
void itoa(int n, char s[])
{
	int i, j, sign;
	enum boolean{
		NO,
		SI
	} acarreo = NO;	/* variable acarreo del tipo enum
				inicializada a NO */

	if((sign = n) < 0)	/* registra el signo */
		n = -(++n);	/* suma 1 y vuelve a n positivo */
		
	i = 0;
	do{		/* genera digitos en orden inverso */
		s[i++] = n % 10 + '0';	/* toma el siguiente digito */	
	}while((n /= 10) > 0);		/* borralo */
	if(sign < 0){
		s[i] = '0';
		j = 0;
		do{
			if(s[j] == '9'){
				s[j++] = '0';
				acarreo = SI;
			}
			else{
				++s[j++];
				acarreo =  NO;
			}
		}while(acarreo);
		if(j > i)
			i = j;
		s[i++] = '-';
	}
	s[i] = '\0';
	
	reverse(s);
}
/* en itoa de la seccion 3.6, el programa falla para
	INT_MIN en la linea n = -n porque en Ca2
	|INT_MIN| = |INT_MAX| + 1 y desborda */

/* reverse: invierte la cadena s en el mismo lugar */
void reverse(char s[])
{
	int c, i, j;

	for(i = 0, j = strlen(s)-1; i < j; i++, j--){
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}