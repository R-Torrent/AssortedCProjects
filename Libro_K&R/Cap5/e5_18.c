/* ejercicio 5_18 */

/* version del programa dcl de la seccion 5.12;
	modificados solamente 'main' y 'gettoken' */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);

int gettoken(void);
int tokentype;
char token[MAXTOKEN];	/* cadena del ultimo token */
char name[MAXTOKEN];	/* nombre del identificador */
char datatype[MAXTOKEN];/* tipo de dato = char, int, etc. */
char out[1000];		/* cadena de salida */

main()	/* convierte una declaracion a palabras */
{
	while(gettoken() != EOF){	/* 1er. token en la linea */
		if(tokentype == '\n')	/* no hace nada mientras no haya entrada */
			continue;
		strcpy(datatype, token);	/* es el tipo de dato */
		out[0] = '\0';
		dcl();			/* reconoce el resto de la linea */
		if(tokentype != '\n'){
			printf("error de sintaxis\n");
			while(gettoken() != '\n')
				;
		}
		else
			printf("%s: %s%s\n", name, out, datatype);
	}
	return 0;
}

int gettoken(void)	/* regresa el siguiente token */
{
	int c, getch(void);
	void ungetch(int);
	char *p = token;

	while((c = getch()) == ' ' || c == '\t')
		;
	if(c == '('){
		while((c = getch()) == ' ' || c == '\t')
			;
		if(c == ')'){
			strcpy(token, "()");
			return tokentype = PARENS;
		}
		else{
			ungetch(c);
			return tokentype = '(';
		}
	}
	else if(c == '['){
		*p++ = c;
		while(c != ']'){
			while((c = getch()) == ' ' || c == '\t')
				;
			if(!isdigit(c) && c != ']'){
				printf("error: falta ]\n");
				return tokentype = c;
			}
			*p++ = c;
		}
		*p = '\0';
		return tokentype = BRACKETS;
	}
	else if(isalpha(c)){
		for(*p++ = c; isalnum(c = getch());)
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	}
	else
		return tokentype = c;
}

#define BUFSIZE 100

char buf[BUFSIZE];	/* buffer para ungetch */
int bufp = 0;		/* siguiente posicion libre en buf */

int getch(void)	/* obtiene un (posiblemente ya regresado) caracter */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)	/* regresa caracter a la entrada */
{
	if(bufp >= BUFSIZE)
		printf("ungetch: demasiados caracteres\n");
	else
		buf[bufp++] = c;
}

/* dcl: reconoce una declaracion */
void dcl(void)
{
	int ns;

	for(ns = 0; gettoken() == '*';)	/* cuenta *s */
		ns++;
	dirdcl();
	while(ns-- > 0)
		strcat(out, "apuntador a ");
}
/* dirdcl: reconoce un declarador directo */
void dirdcl(void)
{
	int type;

	if(tokentype == '('){	/* ( dcl ) */
		dcl();
		if(tokentype != ')')
			printf("error: falta )\n");
	}
	else if(tokentype == NAME)	/* nombre de variable */
		strcpy(name, token);
	else{
		printf("error: nombre o (dcl) esperado\n");
		return;
	}
	while((type = gettoken()) == PARENS || type == BRACKETS)
		if(type == PARENS)
			strcat(out, "funcion que regresa ");
		else{
			strcat(out, "arreglo");
			strcat(out, token);
			strcat(out, " de ");
		}
}