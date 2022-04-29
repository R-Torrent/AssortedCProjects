/* ejercicio 5_11a */
/* version de 'entab' con lista de tabs como argumentos;
	reemplaza blancos por tabuladores */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAB 8	/* 1 '\t' == 8 ' ' */
#define MAXLINE 1000
#define MAXARG 10	/* maximo numero de argumentos */

int getline(char line[], int maxline);
void entab(char *line, int tab_number, int *tab_list, int tab_size);
void intncpy(int *line1, int *line2, int n);

main(int argc, char *argv[])
{
	int i, j, list[MAXARG], actual, tabs = argc - 1;
	char line[MAXLINE];

	if(argc > MAXARG + 1){
		printf("Error: maximo numero de tabuladores %d", MAXARG);
		return -1;
	}			/* rellena el arreglo list si no es por defecto */
	for(i = 0; i < tabs; i++){
		actual = *(list + i) = atoi(*(argv + i + 1));
		if(actual <= 0){
			printf("Error: tabulador %d no valido", actual);
			return -1;
		}
	}
	if(!tabs){	/* caso por defecto */
		for(i = 0; i < MAXARG; i++)
			*(list + i) = TAB * (i + 1);
		tabs = MAXARG;
	}
	else		/* ordenamiento */
		for(i = 0; i < tabs; i++){
			actual = *(list	+ i);			
			for(j = i + 1; j < tabs; ++j)
				if(*(list + j) < actual){
					int temp = *(list + j);
					*(list + j) = actual;
					*(list + i) = actual = temp;
				}	/* eliminamos las repeticiones */
				else if(*(list + j) == actual){
					intncpy(list + j, list + j + 1, tabs - j - 1);
					--tabs, --j;
				}
		}

	while(getline(line, MAXLINE) > 0){
		entab(line, tabs, list, TAB);
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

/* entab: coloca los tabs de list en s
	list ordenado, sin repeticiones */
void entab(char *s, int tabs, int *list, int size)
{
	int j = 1;	/* j: pos en la linea */
	int i, firstblank = 0, maxtab = *(list + tabs - 1);
	char temp[MAXLINE], *ptemp = temp, *ptemp_firstblank; 
	char *s1 = s, actual;
	enum boolean{
		NO,
		SI
	} hubo_tab; 

	while(actual = *s1++){
		if(actual != ' ' && actual != '\t')
			firstblank = 0;
		else if(!firstblank){
			firstblank = j;
			ptemp_firstblank = ptemp;
		}

		hubo_tab = NO;

		do{
			if(j == *list){
				if(firstblank){
					hubo_tab = SI;
					j = firstblank;		/* vuelve atras */
					ptemp = ptemp_firstblank;
					while(j <= *list){
						i = j;
						while(i % size)
							i++;
						if(i <= *list){
							if(j == i)	/* preferencia de un ' ' sobre '\t' */
								*ptemp++ = ' ';
							else{
								*ptemp++ = '\t';
								j = i;
							}
							j++;
						}
						else			/* no cabe un tab */
							do{
								*ptemp++ = ' ';
							}while(j++ != *list);
					}
					firstblank = j--;
					ptemp_firstblank = ptemp;
				}
				list++;
			}
		}while(j++ % size && actual == '\t');

		if(!hubo_tab || j != *(list-1) + 1)
			*ptemp++ = actual;

		if(j > maxtab){
			strcpy(ptemp, s1);
			strcpy(s, temp);
			return;
		}
	}
	*ptemp = '\0';
	strcpy(s, temp);
}

/* intncpy: version de strncat para enteros */
void intncpy(int *s, int *t, int n)
{
	while(n--)
		*s++ = *t++;
}