/* ejercicio 1_23 */
/* elimina los comentarios de un programa en C */

#include <stdio.h>

#define MAXLINE 1000
#define IN 1	/* dentro de una cadena de caracteres */
#define OUT 0	/* fuera de una cadena de caracteres */

int getline(char line[], int maxline);

main()
{
	char line[MAXLINE], sin_com[MAXLINE];
	int comentario, cadena, i, j;

	comentario = cadena = OUT;

	while(getline(line, MAXLINE) > 0){
		
		i = j = 0;
				
		while((sin_com[j]=line[i]) != '\0'){
			if(comentario==IN){
				if(line[i]=='*' && line[i+1]=='/'){
					comentario = OUT;
					++i;
				}
				--j;
			}
			else{
				if(line[i] == '"')
					if(cadena == IN)
						cadena = OUT;
					else
						cadena = IN;

				if(cadena==OUT && line[i]=='/' && line[i+1] =='*'){
					comentario = IN;
					++i;
					--j;
				}
			}						
			++i;
			++j;
		}
		
		printf("%s", sin_com);

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



