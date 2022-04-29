/* ejercicio 6_4 */
/* imprime en orden descendente de frecuencia */

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define MAXWORD 100

struct tnode {			/* el nodo del arbol */
	char *word;			/* apunta hacia el texto */
	int count;			/* numero de ocurrencias */
	struct tnode *left;	/* hijo a la izquierda */
	struct tnode *right;/* hijo a la derecha */
};

int getword(char *, int);
struct tnode *addtree(struct tnode *, char *);
struct tnode *addtree2(struct tnode *, struct tnode *);
void treeprint(struct tnode *);
struct tnode *treerun(struct tnode *,struct tnode *);

main()
{
	struct tnode *root, *root2;
	char word[MAXWORD];

	root = root2 = NULL;

	while(getword(word, MAXWORD) != EOF)
		if(isalpha(word[0]))
			root = addtree(root, word);
	putchar('\n');

	treeprint(treerun(root, root2));

	return 0;
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
char *duplicate(char *);

/* addtree: agrega un nodo con w, en o bajo p */
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;

	if(p == NULL){		/* llego una nueva palabra */
		p = talloc();	/* crea un nuevo nodo */
		p->word = duplicate(w);
		p->count = 1;
		p->left = p->right = NULL;
	}
	else if((cond = strcmp(w, p->word)) == 0)
		p->count++;		/* palabra repetida */
	else if(cond < 0)	/* menor, subarbol izquierdo */
		p->left = addtree(p->left, w);
	else				/* mayor, subarbol derecho */
		p->right = addtree(p->right, w);
	return p;
}

/* treerun: recorre el primer arbol para crear el segundo */
struct tnode *treerun(struct tnode *p, struct tnode *root2)
{
	if(p != NULL){
		root2 = treerun(p->left, root2);
		root2 = treerun(p->right, root2);
		return addtree2(root2, p);
	}
}

/* addtree2: agrega un nodo q, en o bajo p */
struct tnode *addtree2(struct tnode *p, struct tnode *q)
{
	if(p == NULL){		/* llego una nueva palabra */
		p = talloc();	/* crea un nuevo nodo */
		p->word = q->word;
		p->count = q->count;
		p->left = p->right = NULL;
	}
	else if(q->count - p->count > 0)/* mayor, subarbol izquierdo */
		p->left = addtree2(p->left, q);
	else							/* menor o igual, subarbol derecho */
		p->right = addtree2(p->right, q);
	return p;
}

/* treeprint: impresion del arbol p en orden */
void treeprint(struct tnode *p)
{
	if(p != NULL){
		treeprint(p->left);
		printf("%4d %s\n", p->count, p->word);
		treeprint(p->right);
	}
}

/* talloc: crea un tnode */
struct tnode *talloc(void)
{
	return (struct tnode *)malloc(sizeof(struct tnode));
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