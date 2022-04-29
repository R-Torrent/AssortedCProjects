/* ejercicio 6_6 */

/* vesion de '#define' para preprocesador en C; crea una tabla con todas las
	definiciones que encuentra y reemplaza las ocurrencias; no corrige sintaxis
	
	gramatica aceptada:
		#define identificador secuencia-de-tokens
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ID 31	/* 31 caracteres significativos de un nombre */
#define MAX_TOKEN 100
#define MAX_SEC_TOKENS 5050 /* 50 tokens seguidos con 50 espacios */

enum boolean { NO, SI };

enum token { ID, RESW, CONST, CHAIN, OP, OTHER };
enum token gettoken(char *, unsigned short);
void puttoken(char *);

struct nlist {			/* entrada de la tabla */
	struct nlist *next;	/* siguiente entrada en la cadena */
	char *name;			/* nombre definido */
	char *defn;			/* texto de reemplazo */
};

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE];	/* tabla de apuntadores */
unsigned hash(char *);
struct nlist *lookup(char *);
struct nlist *install(char *, char *);
char *strndup(char *, unsigned);

int getch(void);
void ungetch(int);

main()
{
	struct nlist *np;
	char tok[MAX_TOKEN + 1], id[MAX_ID + 1], sec_tok[MAX_SEC_TOKENS + 1];
	enum token type;
	enum boolean prep, define, ext;
		/*	prep: preprocesador
			define: detectado '#define'
			ext: extension, '\\' */
	unsigned h;
	int c, i;

	while((type = gettoken(tok, MAX_TOKEN)) != EOF){
		if(define && *id && *tok != '\\' && *tok != '\n'){
			strcat(sec_tok, tok);
			strcat(sec_tok, " ");
			continue;
		}		
		switch(type){
		case ID:
			if((np = lookup(tok)) != NULL){	/* reemplazar... */
				for(i = 0; c = np->defn[i]; i++)
					ungetch(c);
				continue;
			}
			if(prep)
				if(define){
					if(!*id){
						strncpy(id, tok, MAX_ID);
						id[MAX_ID] = '\0';
					}
				}
				else if(strcmp(tok, "define") == 0){
					define = SI;
					*id = *sec_tok = '\0';
				}
			break;
		case RESW: case CONST: case CHAIN: case OP:
			break;
		case OTHER:
			switch(*tok){
			case('\n'):
				if(prep)
					if(!ext){
						if(define){
							sec_tok[strlen(sec_tok) - 1] = '\0'; /* quita el ultimo ' ' */
							install(id, sec_tok);
						}
					prep = define = NO;
					}
					else
						ext = NO;
				break;
			case('#'):
				prep = SI;
				break;
			case('\\'):
				ext = SI;
				break;
			}
			break;
		}
		puttoken(tok);
	}
	
	printf("\nTabla '#define':\n# hash\tidentificador\tsecuencia dde tokens\n\n");
	for(h = 0; h < HASHSIZE; h++)	/* imprime la tabla */
		if((np = hashtab[h]) != NULL)
			while(np != NULL){
				printf("%d: %s %s\n", h, np->name, np->defn);
				np = np->next;
			}

	return 0;
}

#define NRWORDS (sizeof res_w / sizeof res_w[0])

/* gettoken: obtiene el siguiente componente lexico de la entrada
	ID: identificador
	RESW: palabras reservadas
	CONST: constantes
	CHAIN: cadenas literales
	OP: operadores
	OTHER: otros separadores
	
	puntualizaciones:
	 no tiene en cuenta el prefijo L para conjuntos extendidos de caracteres
	 'lim' inferior operativo = 3 */
enum token gettoken(char *word, unsigned short lim)
{
	int c;
	char *w = word;
	char *binsearch(char *, char **, int);
	char *res_w[] = {
		"auto",
		"break",
		"case",
		"char",
		"const",
		"continue",
		"default",
		"do",
		"double",
		"else",
		"enum",
		"extern",
		"float",
		"for",
		"goto",
		"if",
		"int",
		"long",
		"register",
		"return",
		"short",
		"signed",
		"sizeof",
		"static",
		"struct",
		"switch",
		"typedef",
		"union",
		"unsigned",
		"void",
		"volatile",
		"while"
	};

	while(isspace(c = getch()) && c != '\n')
		;
	if(c == EOF)
		return c;
	*w++ = c;
	lim--;

	switch(c){
	case('\"'):
		while(--lim)
			if((*w++ = getch()) == '\"')
				break;
		*w = '\0';
		return CHAIN;
	
	case('\'')
		if((*w++ = getch()) == '\\'){	/* secuencia de escape */
			int o = 2;
			if(--lim > 1)
				switch(*w++ = c = getch()){
				case 'n': case 't': case 'v': case 'b': case 'r':
				case 'f': case 'a': case '\\': case '?': case '\'':
				case '\"':
					lim--;
					break;
				case '1': case '2': case '3': case '4': case '5':
				case '6': case '7':
					for(; --lim && o--; w++){
						if(!isdigit(*w = c =getch())){
							ungetch(c);
							break;
						}
						if(c > '7')
							return ERR;
					}
					break;
				case 'x':
					for(; --lim; w++)
						if(!isxdigit(*w = c = getch())){
							ungetch(c);
							break;
						}
					break;
				default:
					return ERR;
				}	
		}
		else
			lim--;
		if(lim > 1 && (*w++ = getch()) != '\'')
			return ERR;
		*w = '\0';
		return CONST;
	}	
	else if(isdigit(c)){
		enum { OCT, HEX, FLOAT } const_type = FLOAT;
		/* las constantes de enumeracion se regresan como ID */
		if(c == '0'){	/* octal o hexadecimal */
			if((*w++ = c = getch()) == 'x' || c == 'X')
				const_type = HEX;
			else if(isdigit(c)){
				if(c > '7')
					return ERR;
				const_type = OCT;
			}
			--lim;
		}
		for(; --lim; w++){
			*w = c = getch();
			if(!isdigit(c)){
				if(const_type == HEX && isxdigit(c))
					continue;
				break;
			}
			else if(const_type == OCT && c > '7')
				return ERR;
		}
		if(c == '.'){
			if(const_type != FLOAT)
				return ERR;
			for(; --lim > 0; w++){
				*w = c = getch();
				if(!isdigit(c)){
					ungetch(c);
					break;
				}
			}
		}
		else
			ungetch(c);
		*w = '\0';
		return CONST;
	}
	else if(isalpha(c) ||  c == '_'){
		for(; --lim > 0; w++)
			if(!isalnum(*w = c = getch()) && c != '_'){
				ungetch(c);
				break;
			}
		*w = '\0';
		if(binsearch(word, res_w, NRWORDS) != NULL)
			return ID;
		else
			return RESW;
	}
	else{
		*w = '\0';
		return c;
	}
}

#define BUFSIZE MAX_SEC_TOKENS

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

/* binsearch: encuentra una palabra en tab[0]...tab[n-1] */
char *binsearch(char *word, char **tab, int n)
{
	int cond;
	char **low = tab;
	char **high = tab + n;
	char **mid;

	while(low < high){
		mid = low + (high - low) / 2;
		if((cond = strcmp(word, *mid)) < 0)
			high = mid;
		else if(cond > 0)
			low = mid + 1;
		else
			return *mid;
	}
	return NULL;
}

/* hash: forma un valor hash para la cadena s */
unsigned hash(char *s)
{
	unsigned hashval;

	for(hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

/* lookup: busca s en hashtab */
struct nlist *lookup(char *s)
{
	struct nlist *np;

	for(np = hashtab[hash(s)]; np != NULL; np = np->next)
		if(strcmp(s, np->name) == 0)
			return np;	/* se encontro */
	return NULL;		/* no se encontro */
}

/* install: coloca (name, defn) dentro de hashtab */
struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if((np = lookup(name)) == NULL){	/* no fue encontrado */
		np = (struct nlist *)malloc(sizeof(*np));
		if(np == NULL || (np->name = strndup(name, MAX_ID)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}
	else	/* ya esta alli */
		free((void *)np->defn);	/* libera la anterior defn */
	if((np->defn = strndup(defn, MAX_SEC_TOKENS)) == NULL)
		return NULL;
	return np;
}

/* strndup: crea un duplicado de s de tamano maximo n */
char *strndup(char *s, unsigned lim)
{
	char *p;
	unsigned len;
	
	if((len = strlen(s)) > lim)
		len = lim;
	
	p = (char *)malloc(len + 1); /* + 1`para '\0' */
	if(p != NULL){
		strncpy(p, s, len);
		*(p + len) = '\0';
	}
	return p;
}