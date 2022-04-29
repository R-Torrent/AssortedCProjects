/* ejercicio 6_2 */
/* ordena alfabeticamente nombres de variables */

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define MAXWORD 100
#define LENGTH_DEF 6	/* longitud por defecto */
#define NKEYS (sizeof keytab / sizeof keytab[0])

/* comprueba, comentarios y cadenas constantes */
enum boolean{
	NO,
	SI,
} comentario = NO, cadena = NO;

char *keytab[] = {
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

struct tnode {			/* el nodo del arbol */
	struct list *word;	/* apunta hacia la lista de variable */
	int count;			/* numero de ocurrencias */
	struct tnode *left;	/* hijo a la izquierda */
	struct tnode *right;/* hijo a la derecha */
};

struct list {			/* lista de variables */
	char *name;			/* apunta al nombre */
	struct list *next;	/* siguiente nombre con mismo inicio */
};

int getword(char *, int);
char *binsearch(char *, char **, int);
struct tnode *addtree(struct tnode *, char *, int);
void treeprint(struct tnode *);

main(int argc, char *argv[])
{
	struct tnode *root;
	char word[MAXWORD];
	int n_carac = LENGTH_DEF;	/* # caracteres que determinan las variables */
	int current, last = 0;

	if(argc > 1)
		n_carac = atoi((const char *)*++argv);

	root = NULL;

	while((current = getword(word, MAXWORD)) != EOF){
		if(cadena){
			if(current == '\"')
				cadena = NO;
		}
		else if(comentario){
			if(current == '/' && last == '*')
				comentario = NO;
		}
		else if(isalpha(word[0])){
			if(binsearch(word, keytab, NKEYS) == NULL)
				root = addtree(root, word, n_carac);
		}
		else{
			if(current == '*' && last == '/')
				comentario = SI;
			else if(current == '\"')
				cadena = SI;
		}
		last = current;
	}

	putchar('\n');
	treeprint(root);

	return 0;
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

/* getword: obtiene la siguiente palabra o caracter de la entrada */
int getword(char *word, int lim)
{
	int c, getch(void);
	void ungetch(int);
	char *w = word;

	while(isspace(c = getch()))
		;
	if( c!= EOF)
		*w++ = c;
	if(!isalpha(c)){
		*w = '\0';
		return c;
	}
	for(; --lim > 0; w++)
		if(!isalnum(*w = getch())){
			ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
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

struct tnode *talloc(void);
struct list *lalloc(void);
char *duplicate(char *);
void addlist(struct list *, char *);

/* addtree: agrega un nodo con w, en o bajo p */
struct tnode *addtree(struct tnode *p, char *w, int n)
{
	int cond;

	if(p == NULL){		/* llego una nueva palabra */
		p = talloc();	/* crea un nuevo nodo */
		p->word = lalloc();
		p->word->name = duplicate(w);
		p->word->next = NULL;
		p->count = 1;
		p->left = p->right = NULL;
	}
	else if((cond = strncmp(w, p->word->name, n)) == 0){
		p->count++;		/* palabra repetida */
		addlist(p->word, duplicate(w));
	}
	else if(cond < 0)	/* menor, subarbol izquierdo */
		p->left = addtree(p->left, w, n);
	else				/* mayor, subarbol derecho */
		p->right = addtree(p->right, w, n);
	return p;
}

/* treeprint: impresion del arbol p en orden */
void treeprint(struct tnode *p)
{
	if(p != NULL){
		treeprint(p->left);
		printf("%4d ", p->count);
		for(;;){
			printf("%s", p->word->name);
			if(p->word->next != NULL){
				p->word = p->word->next;
				printf(", ");
			}
			else
				break;
		}
		putchar('\n');
		treeprint(p->right);
	}
}

/* talloc: crea un tnode */
struct tnode *talloc(void)
{
	return (struct tnode *)malloc(sizeof(struct tnode));
}

/* lalloc: crea una entrada de lista */
struct list *lalloc(void)
{
	return (struct list *)malloc(sizeof(struct list));
}

/* duplicate: crea un duplicado de s */
char *duplicate(char *s)
{
	char *p;

	p = (char *)malloc(strlen(s) + 1);	/* +1 para '\0' */
	if(p != NULL)
		strcpy(p, s);
	return p;
}

/* addlist: anade el nombre s a la list l */
void addlist(struct list *l, char *s)
{
	if(strcmp(l->name, s) == 0)	/* encontrado */
		return;
	if(l->next != NULL)			/* sigue buscando */
		addlist(l->next, s);
	else{						/* crea una entrada */
		l->next = lalloc();
		l->next->name = s;
		l->next->next = NULL;
	}
}