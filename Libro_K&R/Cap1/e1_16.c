/* ejercicio 1_16 */

#include <stdio.h>
#define MAXLINE 1000	/* tamaño maximo de la linea de entrada + 1*/
#define IN 1		/* en la 1a porcion de linea larga */
#define OUT 0		/* fuera de la 1a porcion de linea larga */

int getline(char line[], int maxline);
void copy(char to[], char from[]);

/* imprime la linea de entrada mas larga */
main()
{
	int len;			/* longitud de lo traido mediante getline */
	long tlen;			/* longitud actual de la linea */
	int max;			/* maxima longitud vista hasta el momento */
	char line[MAXLINE];		/* linea de entrada actual */
	char incompleta[MAXLINE];	/* aqui se guarda el principio de la linea desbordada */
	char longest[MAXLINE];	/* la linea mas larga se guarda aqui */
	int status;			/* linea desbordada */

	max = tlen = 0;
	status = IN;

	while((len = getline(line, MAXLINE)) > 0){
		if(len==MAXLINE-1 && line[MAXLINE-2]!='\n'){	/* tratamiento para lineas > MAXLINE */
			tlen = tlen + MAXLINE - 1;	/* '- 1' porque en len viene incluido '\0' */ 
			if(status == IN){
				copy(incompleta, line);
				status = OUT;
			}
		}
		else{
			if((tlen + len) > max){
				max = tlen + len;
				if(status == IN)
					copy(longest, line);
				else
					copy(longest, incompleta);
			}
			status = IN;
			tlen = 0;
		}
	}
	if(max > 0)		/* hubo una linea */
		if(max >= MAXLINE)
			printf("\nLa linea fue demasiado larga [MAX: %d].\n", MAXLINE-1);
	printf("Linea mas larga:\n%s", longest);
		printf("\nLongitud: %ld\n", max);
	return 0;
}
/* getline: lee una linea en s, regresa su longitud */
int getline(char s[], int lim)
{
	int c, i;

	for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
		s[i] = c;
	if(c == '\n'){
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}
/* copy: copia 'from' en 'to'; supone que to es suficientemente grande */
void copy(char to[], char from[])
{
	int i;

	i = 0;
	while((to[i] = from[i]) != '\0')
		++i;
}
