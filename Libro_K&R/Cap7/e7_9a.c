/* ejercicio 7_9a */
/* version funcion - ahorra espacio */

#undef isupper

int isupper(int c)
{
	return c >= 'A' && c <= 'Z';
}