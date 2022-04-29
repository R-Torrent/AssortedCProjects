/* ejercicio 6_3 */

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof keytab[0])

/* preposiciones, determinantes (menos numerales) y algunas conjunciones */
char *keytab[] = {
	"a",
	"algun",
	"alguna",
	"algunas",
	"algunos",
	"ante",
	"aunque",
	"bajo",
	"cabe",
	"cada",
	"con",
	"contra",
	"cual",
	"cuales",
	"cuanta",
	"cuantas",
	"cuanto",
	"cuantos",
	"de",
	"desde",
	"el",
	"en",
	"entre",
	"hacia",
	"hasta",
	"la",
	"las",
	"lo",
	"los",
	"mas",
	"mi",
	"mis",
	"mucha",
	"muchas",
	"mucho",
	"muchos",
	"ni",
	"ningun",
	"ninguna",
	"nuestra",
	"nuestras",
	"nuestro",
	"nuestros",
	"o",
	"para",
	"pero",
	"poca",
	"pocas",
	"poco",
	"pocos",
	"por",
	"que",
	"segun",
	"sendas",
	"sendos",
	"sin",
	"so",
	"sobre",
	"su",
	"sus",
	"suya",
	"suyas",
	"suyo",
	"suyos",
	"toda",
	"todas",
	"todo",
	"todos",
	"tras",
	"tu",
	"tus",
	"un",
	"una",
	"unas",
	"unos",
	"varias",
	"varios",
	"vuestra",
	"vuestras",
	"vuestro",
	"vuestros",
	"y"
};

struct tnode {			/* el nodo del arbol */
	char *word;			/* apunta hacia el texto */
	struct list *pages;	/* apunta hacia la lista de lineas */
};

struct list {			/* listas */
	int page;			/* # de linea */
	struct list *n_l;	/* siguiente linea */
	struct tnode *n_w;	/* siguiente palabra */
};

int getword(char *, int);
char *binsearch(char *, char **, int);
struct tnode *addtree(struct tnode *, char *, int);
void treeprint(struct tnode *);

/* imprime el # de linea de todas las palabras de un texto
	(convierte en minusculas) usando estructuras
	referenciadas cruzadas */
main()
{
	struct tnode *root;
	char word[MAXWORD];
	int c, n = 1;	/* n: numero de linea */

	root = NULL;

	while((c = getword(word, MAXWORD)) != EOF)
		if(isalpha(word[0])){
			if(binsearch(word, keytab, NKEYS) == NULL)
				root = addtree(root, word, n);
		}
		else if(c == '\n')
			n++;

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

	while(isspace(c = getch()) && c != '\n')
		;
	if( c!= EOF)
		*w++ = tolower(c);
	if(!isalpha(c)){
		*w = '\0';
		return c;
	}
	for(; --lim > 0; w++)
		if(!isalnum(*w = tolower(getch()))){
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
void addlist(struct list *, int);

/* addtree: agrega un nodo con w, en o bajo p */
struct tnode *addtree(struct tnode *p, char *w, int nump)
{
	if(p == NULL){		/* llego una nueva palabra */
		p = talloc();	/* crea un nuevo nodo */
		p->pages = lalloc();
		p->word = duplicate(w);
		p->pages->page = nump;
		p->pages->n_l = NULL;
		p->pages->n_w = NULL;
	}
	else if(strcmp(w, p->word) == 0)
		/* palabra repetida */
		addlist(p->pages, nump);
	else
		p->pages->n_w = addtree(p->pages->n_w, w, nump);
	return p;
}

/* treeprint: impresion del arbol p en orden */
void treeprint(struct tnode *p)
{
	if(p != NULL){
		printf("%s", p->word);
		for(;;){
			putchar(' ');
			printf("%d", p->pages->page);
			if(p->pages->n_l != NULL){
				p->pages = p->pages->n_l;
				printf(",");
			}
			else
				break;
		}
		putchar('\n');
		treeprint(p->pages->n_w);
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
void addlist(struct list *l, int n)
{
	if(l->page == n)			/* encontrado */
		return;
	if(l->n_l != NULL)			/* sigue buscando */
		addlist(l->n_l, n);
	else{						/* crea una entrada */
		l->n_l = lalloc();
		l->n_l->n_l = NULL;
		l->n_l->n_w = l->n_w;	/* pasa la siguiente palabra */
		l->n_l->page = n;
	}
}