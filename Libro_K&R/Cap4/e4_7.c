/* ejercicio 4_7 */
/* complemento a 'getch' y 'ungetch' del programa e4_3_6 */

/* ungets(s): regresa a la entrada una cadena completa */
void ungets(char s[])
{
	int i;
	for(i = 0; s[i] != '\0'; ++i)
		ungetch(s[i]);	/* solo debe usar ungetch */
}