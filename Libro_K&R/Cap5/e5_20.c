/* ejercicio 5_20 */

/* version extendida del programa dcl de la seccion 5.12 acorde con
	apendice A, seccion 8; puntualizaciones:

	no incluye estructuras, uniones ni enumeraciones;
	no permite inicializadores;
	la opcion declarador-directo ( lista-de-identificadores ) y el
	  especificador de categoria de alamcenamiento 'register' son aceptadas
	  (pese a solo ser correctas encabezando la definicion de una funcion);
	no controla los arreglos multidimensionales
	  (la primera dimension incompleta es aceptada);
	no permite declaradores abstractos como argumentos */
	

 /* gramatica aceptada:

  declaracion:
	especificadores-de-declaracion   lista-de-declaradores
  especificadores-de-declaracion:
	especificador-categoria-almacenamiento   especificadores-de-declaracion_opt
	especificador-de-tipo   especificadores-de-declaracion_opt
	calificador-de-tipo   especificadores-de-declaracion_opt

  lista-declaradores:
	declarador
	lista-declaradores , declarador

  especificador-categoria-almacenamiento:
	auto, register, static, extern

  especificador-de-tipo:
	void, char, short, int, long, float, double, signed, unsigned

  calificador-de-tipo:
	const, volatile

  declarador
	apuntador_opt   declarador-directo
  declarador-directo:
	identificador
	( declarador )
	declarador-directo [ expresion-constante_opt ]
	declarador-directo ( lista-tipos-de-parametro )
	declarador-directo ( lista-de-identificadores_opt )
  apuntador:
	* lista-calificadores-de-tipo_opt
	* lista-calificadores-de-tipo_opt   apuntador
  lista-calificadores-de-tipo:
	calificador-de-tipo
	lista-calificadores-de-tipo   calificador-de-tipo

  lista-tipos-de-parametro:
	lista-de-parametros
	lista-de-parametros , ...
  lista-de-parametros:
	declaracion-parametro
	lista-de-parametros , declaracion-parametro
  declaracion-parametro:
	especificadores-de-declaracion   declarador

  lista-de-indentificadores:
	identificador
	lista-de-indentificadores , identificador */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ENTRADA 100	/* entrada(especificadores de tipo, agumentos...) */
#define MAX_SALIDA 1000	/* descripcion de salida */
#define MAXTOKEN 100
#define MAX_DEC 5		/* max # de declaradores */

void declaracion(char *espec_dec);
void dcl(void);
void dirdcl(void);

enum boolean { NO, SI } error = NO, funcion, req_dim;
/* error: control de error
   req_dim: solo la primera dimension incompleta
   funcion: control de parametro de funcion */

int gettoken(void);
int tokentype;
char token[MAXTOKEN];		/* cadena del ultimo token */
enum ttypes { NAME, NUMBER, PARENS, BRACKETS, NADA } ultimo;

char id[MAX_DEC][MAXTOKEN];	/* identificadores: nombres de variables */
char out[MAX_DEC][MAX_SALIDA];	/* cadenas de salida */
char (*ppid)[MAXTOKEN];
char (*ppout)[MAX_SALIDA];

/* palabras reservadas: */
char *cat_alm[5] = {"auto",	"register",	"static", "extern"};
char *cal_tipo[2] = {"const", "volatile"};
char *tipo[9] = {"void", "float", "char", "int", "double", "short", "long",
	"signed", "unsigned"};
unsigned int reservada(void);
unsigned int espec(char *espec_dec);

main()	/* convierte una declaracion a palabras */
{
	int i, n;					/* n: numero de variables declaradas */
	char espec_dec[MAX_ENTRADA];/* especificadores de declaracion */

	while(gettoken() != EOF){	/* 1er. token */		
		if(tokentype == '\n')	/* no hace nada mientras no haya entrada */
			continue;
		
		/* inicializacion */
		n = 1;
		ppid = id, ppout = out;
		error = NO;
		*espec_dec = '\0';

		while(n <= MAX_DEC && !error){
			**ppout = '\0';
			funcion = NO;
			declaracion(espec_dec);
			if(tokentype != ','){
				if(!error && tokentype != '\n'){
					printf("error: lista de declaradores esperada\n");
					error = SI;
				}
				break;
			}
			gettoken();
			ppid++, ppout++;
			n++;
		}

		if(error)
			while(gettoken() != '\n')
				;
		else
			for(i = 0; i < n; i++)
				printf("%s: %s\b\n", id[i], out[i]);
	}
	return 0;
}

int gettoken(void)	/* regresa el siguiente token */
{
	int c, getch(void);
	void ungetch(int);
	char *p = token;

	*p = '\0';

	while((c = getch()) == ' ' || c == '\t')
		;
	if(c == '(')
		return tokentype = PARENS;
	else if(c == '[')
		return tokentype = BRACKETS;
	else if(isalpha(c)){
		for(*p++ = c; isalnum(c = getch());)
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	}
	else if(isdigit(c)){
		for(*p++ = c; isdigit(c = getch());)
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NUMBER;
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

/* captura los especificadores de declaracion (tipos, calificadores...)
	regresa espec_flags, 0 si hay error */
unsigned int espec(char *espec_dec)
{
	unsigned int i, espec_flags = 0;	
	/* flags de los especificadores:
			bit 0: categoria de almacenamiento
			bit 1: calificador de tipo
			especificadores de tipo:
				bit 2: void, float
				bit 3: char
				bit 4: int
				bit 5: double
				bit 6: short
				bit 7: long
				bit 8: signed, unsigned */

	while(i = reservada()){
		if(espec_flags & 1 & i)				
			printf("error: %s, categoria de almacenamiento ya especificada\n", token);
		else if((espec_flags & 0x3C) && (i & 0x3C)) 
			printf("error: %s, tipo ya especificado\n", token);
		else if((espec_flags & 0xC0) && (i & 0xC0) ||
				(espec_flags & 0x100 & i))
			printf("error: %s redundante\n", token);
		else if(((espec_flags | i) & 0x6C) > 0x40)
			printf("error: tipo short aplicable a int\n");
		else if(((espec_flags | i) & 0x8C) > 0x80)
			printf("error: tipo long aplicable a int y double\n");
		else if(((espec_flags | i) & 0x124) > 0x100)
			printf("error: tipo signed/unsigned aplicable a char e int\n");
		else{
			espec_flags |= i;
			if(i != 2 || strstr(espec_dec, token) == NULL)
				/* quita los calificadores de tipo redundantes */
				strcat(strcat(espec_dec, token), " ");
			gettoken();
			continue;
		}
		return 0;
	}
	if(!(espec_flags & 0x1FC)){
		printf("error: falta un especificador de tipo\n");
		return 0;
	}
	return espec_flags;
}

/* reservada: comprueba si 'token' es una palabra reservada;
	regresa el bit adecuado para 'espec_flags' */
unsigned int reservada(void)
{
	int i;

	for(i = 0; i < 4; i++)
		if(!strcmp(cat_alm[i], token))
			return 1;
	for(i = 0; i < 2; i++)
		if(!strcmp(cal_tipo[i], token))
			return 2;
	if(!strcmp(tipo[0], token) || !strcmp(tipo[1], token))
		return 4;
	if(!strcmp(tipo[2], token))
		return 8;
	if(!strcmp(tipo[3], token))
		return 0x10;
	if(!strcmp(tipo[4], token))
		return 0x20;
	if(!strcmp(tipo[5], token))
		return 0x40;
	if(!strcmp(tipo[6], token))
		return 0x80;
	if(!strcmp(tipo[7], token) || !strcmp(tipo[8], token))
		return 0x100;

	return 0;	/* no es palabra reservada */
}

void declaracion(char *espec_dec)
{
	unsigned int i;

	/* captura los especificadores de declaracion */
	if(*espec_dec || (i = espec(espec_dec))){
		if(funcion && i & 1 && strstr(espec_dec, "register") == NULL){
			printf("error: especificador de categoria distinto a register\n");
			error = SI;
			return;
		}
		/* captura el declarador de variable */
		if(reservada()){
			printf("error: uso de %s como identificador\n", token);
			error = SI;
			return;
		}
		req_dim = NO;
		ultimo = NADA;
		dcl();
		strcat(*ppout, espec_dec);
	}
	else
		error = SI;
}
	
/* dcl: reconoce una declaracion */
void dcl(void)
{
	unsigned int i;

	if(tokentype == '*'){
		/* reconoce un apuntador */
		char list_cal[MAX_ENTRADA] = "apuntador(es) ";
		while(gettoken() == NAME && (i = reservada()))
			if(i == 2){
				/* quita los calificadores de tipo redundantes */
				if(strstr(list_cal, token) == NULL)
					strcat(strcat(list_cal, token), " ");
			}
			else{
				printf("error: uso de %s como identificador\n", token);
				error = SI;
				return;
			}
		dcl();
		req_dim = NO;
		ultimo = NADA;
		strcat(strcat(*ppout, list_cal), "a ");
		return;
	}
	dirdcl();
}

/* dirdcl: reconoce un declarador directo */
void dirdcl(void)
{
	if(tokentype == PARENS){	/* ( dcl ) */
		gettoken();
		dcl();
		if(tokentype != ')'){
			printf("error: falta )\n");
			error = SI;
		}
	}
	else if(tokentype == NAME){	/* nombre de variable */
		if(!funcion)
			strcpy(*ppid, token);
	}
	else{
		printf("error: identificador o (dcl) esperado\n");
		error = SI;
	}
	
	while(gettoken() == PARENS || tokentype == BRACKETS){
		if(tokentype == PARENS){
			if(ultimo == BRACKETS){
				printf("error: el tipo de un arreglo no puede ser funcion\n");
				error = SI;
			}
			funcion = SI;
			strcat(*ppout, "funcion ");
			if(gettoken() == NAME){
				char temp[MAX_ENTRADA];
				if(reservada()){
					/* f(lista-tipos-de-parametro) */
					strcat(*ppout, "de parametro(s) ");
					for(;;){
						*temp = '\0';
						if(tokentype == '.'){
							if(gettoken() == '.' && gettoken() == '.'){
								strcat(*ppout, "... ");
								gettoken();
							}
							else{
								printf("error: '...' esperado\n");
								error = SI;
							}
							break;
						}
						declaracion(temp);
						if(tokentype != ',' || error)
							break;
						*(*ppout + strlen(*ppout) - 1) = '\0'; /* elimina ' ' */
						strcat(*ppout, ", ");
						gettoken();
					}
					*(*ppout + strlen(*ppout) - 1) = '\0'; /* elimina ' ' final */
					strcat(*ppout, "; ");
				}
				else{
					/* f(lista-de-identificadores) */
					int n_id;
					for(n_id = 1; gettoken() == ','; n_id++)
						if(gettoken() != NAME){
							printf("error: falta un identificador\n");
							error = SI;
							break;
						}
						else if(reservada()){
							printf("error: uso de %s como identificador\n", token);
							error = SI;
							break;
						}
					sprintf(temp, "de %d identificadores ", n_id);
					strcat(*ppout, temp);
				}
				if(tokentype != ')'){
					printf("error: falta )\n");
					error = SI;
				}
			}
			else if(tokentype == ')')
				/* f() */
				strcat(*ppout, "de argumentos no especificados ");
			else{
				printf("error: falta ) antes de %c\n", tokentype);
				error = SI;
			}
			strcat(*ppout, "que regresa ");
			ultimo = PARENS;
		}
		
		else{
			if(ultimo == PARENS){
				printf("error: una funcion no puede regresar un arreglo\n");
				error = SI;
			}
			strcat(*ppout, "arreglo[");
			gettoken();
			if(tokentype == NUMBER){
				strcat(*ppout, token);
				gettoken();
			}
			else if(tokentype == ']')
				if(!req_dim)
					strcat(*ppout, "incompleto");
				else{
					printf("error: falta un subindice\n");
					error = SI;
				}
			if(tokentype != ']'){
				printf("error: falta ]\n");
				error = SI;
			}
			req_dim = SI;
			strcat(*ppout, "] de ");
			ultimo = BRACKETS;
		}

		if(error)
			return;
	}
}