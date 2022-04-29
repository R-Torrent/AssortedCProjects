/* ejercicio 1_21 */
/* PROGRAMA entab
	 reemplaza espacios en blanco por tabuladores */

#include <stdio.h>
#define TAB 8	/* 1 '\t' == 8 ' ' */
#define MAXLINE 1000

int getline(char line[], int maxline);
void copy(char to[], char from[]);
void entab(char line[], int tab_space);

main()
{
	
	char line[MAXLINE];

	while(getline(line, MAXLINE) > 0){
		entab(line, TAB);
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

void entab(char s[], int lim)
{
	int d;		/* almacena la distancia hasta el siguiente \t */
	int i, j;	/* i: pos en s; j: pos en temporal */
	int c;		/* contador de espacios seguidos */
	char temporal[MAXLINE], actual;

	i = j = 0;
	d = lim;

	while((actual = s[i]) != '\0' && j != MAXLINE-1){
		if(actual == ' '){ 
			for(c = 1; s[c+i] == ' '; ++c)	/* aqui cuenta */
				;
			i = i + c;
			while(c>0){			/* y aqui substituye */
				if(c > d){		/* Cuando un espacio es suficiente para
								alcanzar el paro de tabulacion
								tiene preferencia ' ' sobre '\t',
								'c >= d' da preferencia a '\t' sobre ' ' */
					temporal[j] = '\t';
					++j;
					c = c - d;
					d = lim;
				}
				else while(c > 0){
					temporal[j] = ' ';
					++j;
					--c;
					--d;
				}
			}
		}
						
		else{
			temporal[j] = actual;
			++j;
			++i;
			--d;
		}
		
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

		
		