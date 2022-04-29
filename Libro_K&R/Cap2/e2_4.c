/* ejercicio 2_4 */
/* 2a version de squeeze(s1, s2) */

#include <stdio.h>
#define MAX 1000

void squeeze(char cadena[], char car_borrar[]);
int getcadena(char cadena[], int max);

main()
{
	char s1[MAX], s2[MAX];

	printf("Cadena: ");
	getcadena(s1, MAX);
	printf("Caracteres a borrar: ");
	getcadena(s2, MAX);

	squeeze(s1, s2);
	printf("Resultado: %s\n", s1);

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

/* squeeze: borra todos los caracteres de s2 que
	aparezcan en s1 */
void squeeze(char s1[], char s2[])
{
	int i, j, k;
	
	for(k=0; s2[k]!='\0'; ++k){
		for(i=j=0; s1[i]!='\0'; ++i)
			if(s1[i]!=s2[k])
				s1[j++] = s1[i];
		s1[j] = '\0';
	}
}