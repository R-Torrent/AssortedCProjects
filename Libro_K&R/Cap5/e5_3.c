/* ejercicio 5_3 */
/* version de 'strcat' usando apuntadores */

/* strcat: concatena t al final de s; s debe ser suficientemente grande */
void strcat(char *s, char *t)
{
	while(*s)	/* avanza hasta el fin de s */
		s++;
	while(*s++ = *t++)
		;
}
	
