/* ejercicio 5_19 */

/* version del programa undcl de la seccion 5.12 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

int gettoken(void);
char token[MAXTOKEN];	/* cadena del ultimo token */
char out[1000];			/* cadena de salida */

/* undcl: convierte una descripcion verbal a declaracion */
main()
{
	int type, type_ant;
	char temp[MAXTOKEN];

	while(gettoken() != EOF){
		strcpy(out, token);
		type_ant = -1;
		while((type = gettoken()) != '\n'){
			if(type == PARENS || type == BRACKETS){
				if(type_ant == '*'){
					sprintf(temp, "(%s)", out);
					strcpy(out, temp);
				}
				strcat(out, token);
			}
			else if(type == '*'){
				sprintf(temp, "*%s", out);
				strcpy(out, temp);
			}
			else if(type == NAME){
				sprintf(temp, "%s %s", token, out);
				strcpy(out, temp);
			}
			else
				printf("entrada invalida en %s\n", token);
			type_ant = type;
		}
		printf("%s\n",out);
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
			return PARENS;
		}
		else{
			ungetch(c);
			return '(';
		}
	}
	else if(c == '['){
		for(*p++ = c; (*p++ = getch()) != ']';)
			;
		*p = '\0';
		return BRACKETS;
	}
	else if(isalpha(c)){
		for(*p++ = c; isalnum(c = getch());)
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return NAME;
	}
	else
		return c;
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