/* ejercicio 2_3 */
/* convierte una cadena de digitos hexadecimales
	en su valor entero equivalente */

#include <stdio.h>
#include <ctype.h>
#define MAX 10 	/* numero maximo de digitos hexadecimales + 3
					(0x al principio y '\0' al final) */

int htoi(char hexadecimal[]);
int gethex(char hexadecimal[], int max);

main()
{
	char number[MAX];
	int n;

	gethex(number, MAX);
	printf("%s", number);

	if((n=htoi(number)) >= 0)
		printf(": %u\n", n); 
	else{
		printf(" no es hexadecimal");
		printf("\nformato: 0x#######\n");
	}
	
	return 0;
}

/* gethex: lee un numero en s, regresa su longitud */
int gethex(char s[], int lim)
{
	int c, i;

	for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
		s[i] = c;
	s[i] = '\0';
	
	return i;
}

/* htoi:convierte una cadena hexadecimal en un entero
	regresa el valor, o -1 si se produce un error
	solo funciona en ASCII */
int htoi(char s[])
{
	int i;
	int n = 0;
	
	for(i=0; s[i]!='\0'; ++i)	/* cambia todo a mayusculas */
		s[i] = toupper(s[i]);

	for(i = 0; s[i]!='\0'; ++i){
		if((!isxdigit(s[i]) && i!=1) || (s[i]!='X' && i==1))
			return -1;
		if(i==0 && s[i]!='0')
			return -1;
		n = 16 * n;
		if(i>1)
			if(isdigit(s[i]))
				n = n + (s[i] - '0');
			else
				n = n + (s[i] - 'A' + 10);
	}		

	return n;
}