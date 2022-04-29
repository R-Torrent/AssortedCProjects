/* ejercicio 1_22 */
/* divide lineas largas en otras mas cortas */

#include <stdio.h>

#define MAXLINE 1000
#define N 20	/* columna n-esima de la entrada */
#define TAB 8	/* 1 '\t' == 8 ' ' */

/* Opciones de 'elim_blanks':
	D: delanteros
	T: traseros
	A: ambos	*/		
#define D 1		
#define T 2		
#define A 3		

int pos;	/* variable externa con la posicion
			actual en el arreglo 'line[]' */
int getline(char line[], int maxline);
int divide(char line2[], char line[], int maxline2);
void copy(char to[], char from[]);
void detab(char line[], int tab_space);
void entab(char line[], int tab_space);
int elim_blanks(char line[], int opcion); 

main()
{
	extern int pos;
	char line[MAXLINE], parte[N+2]; /* N + '\n' + '\0' = N+2 */

	while(getline(line, MAXLINE) > 0){
		
		pos = 0;

		detab(line, TAB);
		
		while(divide(parte, line, N) > 0){
			entab(parte, TAB);
			printf("%s", parte);
		}
	
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

/* divide: lee una linea en s, devuelve la primera parte
	para que quepa en la columna 'lim' de la pantalla.
	Añade '\0' y '\n' (si procede) y devuelve el tamaño
	(sin contar '\0') de la parte. */
int divide(char t[], char s[], int lim)
{
	extern int pos;
	int i;
	char actual;

	i = 0;

	while((actual = t[i] = s[pos]) != '\0'){
		++pos;
		if(actual == '\n' || i == lim-1){
			if(actual != '\n'){
				++i;
				if(s[pos] == '\n')
					++pos;
				t[i] = '\n';
			}						
			t[i+1] = '\0';
			return elim_blanks(t, T);
		}
		++i;
	}

	return elim_blanks(t, T);
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

/* elim_blanks: elimina los espacios en blanco
	y tabs; devuelve la longitud */
elim_blanks(char s[], int opcion)
{
	int i, j;

	if(opcion != T){		/* espacios/tabs delanteros */
		for(j = 0; s[j] != '\0' && (s[j]==' ' || s[j]=='\t'); ++j)
			;
		for(i = 0; (s[i]=s[i+j]) != '\0'; ++i)
				;
	}

	if(opcion != D){		/* espacios/tabs traseros */
		if(opcion == T)
			for(i = 0; s[i] != '\0'; ++i)	/* tamaño de 's[]' */
				;
		while(i > 1 && (s[i-2]==' ' || s[i-2]=='\t'))
			--i;
		if(i > 0)
			s[i-1] = '\n';
		s[i] = '\0';
	}

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