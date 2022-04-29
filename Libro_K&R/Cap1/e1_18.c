/* ejercicio 1_18 */
/* elimina espacios y tabs al final de cada linea y
	lineas en blanco */

#include <stdio.h>
#define MAXLINE 1000

int getline(char line[], int maxline);

main()
{
	int len, i;
	char line[MAXLINE];

	while((len = getline(line, MAXLINE)) > 0){
		i = len - 1;
		while(i > 0 && (line[i-1]==' ' || line[i-1]=='\t'))
			--i;
		line[i] = '\n';
		line[i+1] = '\0';
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