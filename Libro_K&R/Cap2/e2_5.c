/* ejercicio 2_5 */
/* encontrar la primera ocurrencia de cualquier caracter
	de s2 en s1 */

#include <stdio.h>
#define MAX 1000

int any(char cadena[], char car_encontrar[]);
int getcadena(char cadena[], int max);

main()
{
	char s1[MAX], s2[MAX];

	printf("Cadena: ");
	getcadena(s1, MAX);
	printf("Caracteres a encontrar: ");
	getcadena(s2, MAX);

	printf("Resultado: %d\n", any(s1, s2));

	return 0;
}

/* getcadena: lee una cadena en s, regresa su longitud */
int getcadena(char s[], int lim)
{
	int c, i;

	for (i=0; i<lim-1 && (c=getchar())!=EOF; ++i)
		s[i] = c;	
	s[i] = '\0';

	return i;
}

/* any: version de strpbrk
	regresa un entero en vez de un apuntador */
int any(char s1[], char s2[])
{
	int i, j, n;

	n = -2; /* si no encuentra, regresa -1 */
	
	for(j=0; s2[j]!='\0'; ++j)
		for(i=0; s1[i]!='\0'; ++i)
			if(s1[i]==s2[j] && (i<n || n==-2))
				n = i;

	return n + 1;
}