/* ejercicio 5_4 */

/* strend: regresa 1 si la cadena t esta al final de s;
	0 si no es asi */
int strend(char *s, char *t)
{
	int i, j;
	i = j = 0;

	while(s[i])	/* avanza hasta el fin de s */
		i++;
	while(t[j])	/* avanza hasta el fin de t */
		j++;
	if(j > i || j == 0)	/* error: t > s || t == "\0" */
		return 0;

	s += i - j;
	while(*s++ == *t++)
		if(!*s)
			return 1;
	return 0;
}