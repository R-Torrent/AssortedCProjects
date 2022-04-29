/* ejercicio 1_19 */
/* escribir funcion que invierta la cadena de caracteres 
	'reverse(s)' */

#include <stdio.h>

#define MAXLINE 1000

int getline(char line[], int maxline);
void copy(char to[], char from[]);
void reverse(char line[]);

main()
{
	int len;
	char line[MAXLINE];

	while((len = getline(line, MAXLINE)) > 0){
		reverse(line);
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

/* copy: copia 'from' en 'to'; supone que to es suficientemente grande */
void copy(char to[], char from[])
{
	int i;

	i = 0;
	while((to[i] = from[i]) != '\0')
		++i;
}

/* reverse: invierte el orden en una cadena de caracteres
	permanecen en su lugar '\n' y '\0' */
void reverse(char s[])
{
	char temporal[MAXLINE];
	int limite, i;

	limite = i = 0;

	while(s[limite] != '\0')	/* encuentra el final */
		++limite;

	while(i != limite-1){
		temporal[i] = s[limite-i-2];
		++i;
	}
	temporal[limite-1] = '\n';
	temporal[limite] = '\0';

	copy(s, temporal);
}