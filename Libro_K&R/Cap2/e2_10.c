/* ejercicio 2_10 */

/* lower: convierte c a minuscula; solamente ASCII */
int lower(int c)
{
	return (c >= 'A' && c <= 'Z') ? c+'a'-'A' : c;
}