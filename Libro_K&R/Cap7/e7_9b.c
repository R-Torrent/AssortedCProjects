/* ejercicio 7_9b */
/* version macro - acelera la ejecucion */

#undef isupper
#define isupper(c) ((c) >= 'A' && (c) <= 'Z')