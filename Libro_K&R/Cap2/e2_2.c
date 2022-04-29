/* ejercicio 2_2 */
/* version de la rutina "getline" del cap 1
	 sin usar "&&" ni "||" */

#include <stdio.h>
#define MAXLINE 1000	/* tamaño maximo de la linea de entrada + 1 */
int getline(char line[], const int maxline);

main()
{
	char line[MAXLINE];

	printf("%d", getline(line, MAXLINE));

	return 0;
}

int getline(char s[], const int lim)
{
	int c, i = 0;
	enum boolean{
		NO,
		SI
	} dentro = SI;	/* variable dentro del tipo enum
					inicializada a SI */
	while(dentro)
		if(i>=lim-1)
			dentro = NO;
		else if((c=getchar()) == EOF)
			dentro = NO;
		else{
			if(c == '\n')
				dentro = NO;
			s[i] = c;
			++i;			
		}
	
	s[i] = '\0';

	return i;
}
