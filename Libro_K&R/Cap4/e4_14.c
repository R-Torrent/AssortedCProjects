/* ejercicio 4_14 */
/* definicion de macro 'swap(t, x, y)' */

#define swap(t, x, y) { \
	t c; /* declaracion dentro de bloque */ \
	c = x, x = y, y = c; \
}