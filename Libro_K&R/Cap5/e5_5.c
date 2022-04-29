/* ejercicio 5_5 */
/* versiones de 'strncpy', 'strncat', 'strncmp' */

/* strncpy: copia hasta n caracters de t hacia s
	regresa s */
char *strncpy(char *s, char *t, int n)
{
	char *s1 = s;

	while((*s++ = *t++) && s-s1 < n)
		;
	return s1;
}

/* strncat: concatena hasta n caracters	de t a s
	regresa s */
char *strncat(char *s, char *t, int n)
{
	char *s1 = s;

	while(*s)	/* avanza hasta el fin de s */
		s++;
	strncpy(s, t, n);	
	
	return s1;
}

/* strncmp: compara hasta n caracteres
	regresa <0 si s<t, 0 si s==t, >0 si s>t */
int strncmp(char *s, char *t, int n)
{
	int i;

	for(i = 0; *s == *t && i < n-1; ++i, ++s, ++t)
		if(!*s)		/* s[i] == '\0' */
			return 0;
	return *s - *t;
}