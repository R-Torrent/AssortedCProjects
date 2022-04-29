/* ejercicio 3_6 */

#include <string.h>

/* itoa2: convierte n a caracteres en s; ancho minimo m */
void itoa2(int n, char s[], int m)
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
	for(; i < m; ++i)
		s[i] = '0';
	s[i] = '\0';
	
	reverse(s);
}

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