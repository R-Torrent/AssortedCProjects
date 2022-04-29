/* ejercicio 6_5 */

#include <stdlib.h>
#include <string.h>

struct nlist {			/* entrada de la tabla */
	struct nlist *next;	/* siguiente entrada en la cadena */
	char *name;			/* nombre definido */
	char *defn;			/* texto de reemplazo */
};

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE];	/* tabla de apuntadores */

unsigned hash(char *);

/* undef: borra un nombre y una definicion de la tabla hash
	regresa la posicion de la entrada anterior o NULL */
struct nlist *undef(char *s)
{
	struct nlist *np, *ante_np;
	unsigned hashval = hash(s);

	ante_np = np = hashtab[hashval];

	while(np != NULL && strcmp(s, np->name) != 0){
		ante_np = np;
		np = np->next;
	}
	if(np == NULL)		/* no se encontro */
		return NULL;
	if(ante_np == np)	/* principio de lista */
		hashtab[hashval] = np->next;
	else
		ante_np->next = np->next;

	free((void *)np);	/* libera el espacio */
	return ante_np;
}