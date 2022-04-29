/* ejercicio 1_20 */
/* PROGRAMA detab
	 reemplaza tabuladores por espacios en blanco */

#include <stdio.h>
#define TAB 8	/* 1 '\t' == 8 ' '
			este es el 'n' al que hace mencion el ejercicio */
#define MAXLINE 1000

int getline(char line[], int maxline);
void copy(char to[], char from[]);
void detab(char line[], int tab_space);

main()
{
	
	char line[MAXLINE];

	while(getline(line, MAXLINE) > 0){
		detab(line, TAB);
		printf("%s", line);
	}
	
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

void detab(char s[], int lim)
{
	int d;		/* almacena la distancia hasta el siguiente \t */
	int i, j;	/* i: pos en s; j: pos en temporal */
	char temporal[MAXLINE], actual;

	i = j = 0;
	d = lim;

	while((actual = s[i]) != '\0'){
		if(actual == '\t') 
			while(d>0){
				temporal[j] = ' ';
				++j;
				--d;
			}
		else{
			temporal[j] = actual;
			++j;
			--d;
		}
		++i;
		if(d == 0)
			d = lim;
	}
	temporal[j] = actual;
	copy(s, temporal);
}
	
/* copy: copia 'from' en 'to'; supone que to es suficientemente grande */
void copy(char to[], char from[])
{
	int i;

	i = 0;
	while((to[i] = from[i]) != '\0')
		++i;
}

		
		