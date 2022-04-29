/* ejercicio 3_5 */

#include <string.h>

/* itob: convierte n (entero base 10) a caracteres en s en base b */
void itob(int n, char s[], int b)
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
		s[i] = n % b;	/* toma el siguiente digito */
		if(s[i] > 10)	/* para bases > 10, tomamos letras */
			s[i++] += 'a' - 10;
		else
			s[i++] += '0';
	}while((n /= b) > 0);		/* borralo */
	if(sign < 0){
		s[i] = '0';
		j = 0;
		do{
			if(s[j] == ((b > 10) ? b-1+'a'-10 : b-1+'0')){
				s[j++] = '0';
				acarreo = SI;
			}
			else{
				if(s[j] == '9')
					s[j++] = 'a';
				else
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