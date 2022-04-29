/* Fracciones continuas

   Sintaxis: fracc p q [\c[:##]] [\r]
     donde 'p' es el dividendo y 'q' el divisor (!=0); magnitudes del tipo 'signed int'
     indicador opcional '\c' para imprimir hasta ## convergentes (todos, si falta':##')
     indicador opcional '\r' para operar con la raíz cuadrada de p / q		** PROYECTADO **
   Devuelve la fracción continua de p / q en formato canónico
   Programa conforme al estándar C99

   Este código está escrito con tabuladores = 8 espacios

   Roger Torrent
   05 de abril de 2020
*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define CNV 001 // opción \c imprimir convergentes
#define RZC 002 // opción \r raíz cuadrada

#define SIZE 20
#define MIN(A, B) ((A) < (B) ? (A) : (B))

// Max # pasos en el algoritmo de Euclides (Lamé, 1845)
#define MEuclides (int)ceil(4.784972*log10(abs(INT_MAX)) - 0.327724)

void printFRACCn(int, const int * restrict);

int main(int argc, char *argv[])
{
  int p, q, i = 2;
  struct {
    int nc;		// número de convergentes
    unsigned char flgs;	// banderas optativas
  } opcs = {0};		// 0 convergentes == todo
  char buffer[SIZE];

  if (argc < 3 || !(p = atoi(argv[1]), q = atoi(argv[2]))) goto error;
  sprintf(buffer, "%d", p);
  if (strcmp(argv[1], buffer) && (*argv[1] != '+' || strcmp(argv[1] + 1, buffer)))
    goto error;		// error: dividendo no entero
  sprintf(buffer, "%d", q);
  if (strcmp(argv[2], buffer) && (*argv[2] != '+' || strcmp(argv[2] + 1, buffer)))
    goto error;		// error: divisor no entero
  while (++i < argc) {	// argumentos optativos
    char *pargv = argv[i];
    if (*pargv++ != '\\')
      goto error;	// error: esperaba '\'
    switch (*pargv++) {
      case 'c': case 'C':
	if (*pargv) {
	  if (*pargv++ != ':')
	    goto error;	// error: esperaba ':'
	  sprintf(buffer, "%d", opcs.nc = atoi(pargv));
	  if (strcmp(pargv, buffer))
	    goto error; // error: esperaba ## número entero
	  if (opcs.nc < 1)
	    goto error;	// error: esperaba ## > 0 convergentes
        }
	opcs.flgs |= CNV;
	break;
      case 'r': case 'R':
	if (*pargv)
	  goto error;	// error: opcion ilegal
	opcs.flgs |= RZC;
	break;
      default:
	goto error;	// error: opcion ilegal
    }
    continue;
error:
    fprintf(stderr, "SYNTAX ERROR: %s <\"dividend\"> <\"divisor\"> [\\c[:##]] [\\r]\n", *argv);
    fprintf(stderr, "\twith \"divisor\" != 0, ## > 0,\n");
    fprintf(stderr, "\tand |arguments| <= %d\n", INT_MAX);
    exit(EXIT_FAILURE);
  }

  int a[MEuclides], *pa = a;
  div_t op;		// tipo 'div_t' definido en <stdlib.h>
  do {
    if ((p > 0 && q < 0) || (p < 0 && q > 0)) {	// sólo se puede cumplir en la parte integral;
      op.quot = (int)floor((double)p / q);	// para la siguiente iteración, sgn(divisor) = sgn(resto)
      op.rem = p - op.quot * q;
    }
    else
      op = div(p, q);	// función 'div' definida en <stdlib.h>
    *pa++ = op.quot;
    p = q;
    q = op.rem;
  } while (q);
  int n = pa - a;	// cantidad de elementos

  if (opcs.nc) n = MIN(n, opcs.nc);
  int pk[n+2], qk[n+2];
  pk[0] = 0; pk[1] = 1;
  qk[0] = 1; qk[1] = 0;
  for (i = 0; i < n; ) {
    p = pk[i+2] = a[i]*pk[i+1] + pk[i];
    q = qk[i+2] = a[i]*qk[i+1] + qk[i];
    if(++i == n || opcs.flgs & CNV) {
      printFRACCn(i, a);
      printf("  = %d / %d\n", p, q);
      }
  }

/* FALTA PROGRAMAR EL CÁLCULO DE LAS RAÍCES CUADRADAS: opcs.flgs & RZC
   PODRÍA RESOLVERSE SQRT(p/q)
   O EXPANDIR LA OPCIÓN \r:## PARA ABARCAR (p+SQRT(r))/q */

  exit(EXIT_SUCCESS);
}

void printFRACCn(int n, const int a[restrict static n])
{
  const int *a0 = a;
  bool pyc = *a;

  printf("[");
  if (pyc++ || n == 1)
    printf("%d;%s", *a, n > 1 ? " " : "");
  for(a++; a - a0 < n; a++)
    printf("%s%d", !pyc ? ", " : (pyc=false, ""), *a);
  printf("]\n");

  return;
}