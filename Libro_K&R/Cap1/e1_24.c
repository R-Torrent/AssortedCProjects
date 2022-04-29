/* ejercicio 1_24 */
/* elimina errores de sintaxis rudimentarios de un programa en C */

/*      
incluye:        parentesis, llaves, y corchetes no alineados
			comillas, apostrofos, secuencias de escape y comentarios
funcionamiento:
			1. copia el texto a un temporal
				(todo el trabajo sobre este)
			2. elimina los comentarios
			3. comprueba secuencias de escape dentro de
				apostrofes y comillas
			4. elimina las cadenas y constantes de caracteres
			5. comprueba la alineacion de corchetes,
				parentesis y llaves (en este orden)
secuencias de escape reconocidas: (de la seccion 1.1 del libro)
	\n, \t, \b, \", \\ y \0
*/

#include <stdio.h>

#define MAXTEXT 10000   /* capturaremos todo el texto de golpe y
			despues haremos el tratamiento de errores */
#define IN 1    /* dentro */
#define OUT 0   /* fuera */
#define MAXANID 20		/* maximo anidamiento de llaves */

int error;
int gettext(char text[], int maxtext);
int com_c(char text[]);

main()
{
	char text[MAXTEXT];
	extern int error;

	error = 0;

	if(gettext(text, MAXTEXT) > 0)         /* hay texto */

		com_c(text);

	printf("\nNumero de errores: %d\n", error);

	return 0;
}

/* gettext: lee un texto en s, regresa su longitud */
int gettext(char s[], int lim)
{
	int i;

	for (i=0; i<lim-1 && (s[i]=getchar())!=EOF; ++i)
		;
	s[i] = '\0';

	return i;
}

/* com_c: elimina los comentarios de un programa
	en C; comprueba las secuencias de escape;
	elimina las cadenas y constantes de carecteres;
	comprueba la alineacion de llaves, parentesis
	y corchetes; regresa su longitud */
int com_c(char s[])
{
	int i, j, linea;
	int comentario, cadena, constante, barra;
	int plla, mlla[MAXANID], par, cor;
	
	extern int error;

	comentario = cadena = constante = barra = cor = OUT;
	i = plla = par = 0;
	linea = 1;

	while(s[i] != '\0'){
		
		if(s[i]=='\n'){         /* para saber en que linea se produce el error */
			if(cadena==IN){         /* la secuencia de escape \' es del cap. 2... */
				printf("\nError: Falta \'\"\' en la linea %d", linea);
				++error;
				cadena = OUT;
			}
			else if(constante==IN){
				printf("\nError: Falta \'\'\' en la linea %d", linea);
				++error;
				constante = OUT;
			}
			if(cor==IN){
				printf("\nError: Falta \']\' en la linea %d", linea);
				++error;
				cor = OUT;
			}
			if(par>0){
				for(j=0; j<par; ++j){
					printf("\nError: Falta \')\' en la linea %d", linea);
					++error;
				}
				par = 0;
			}
			++linea;
		}
		
		if(comentario==IN && s[i]=='*' && s[i+1]=='/'){
			comentario = OUT;
			++i;
		}

		if(s[i]=='/' && s[i+1]=='*' && comentario==OUT){
			comentario = IN;
			++i;
		}

		if(comentario==OUT)

			if(s[i]=='\\')
				if(barra==IN)   /* barra sirve para controlar '\\' y '\"' */
					barra = OUT;
				
				else{
					barra = IN;
					if(cadena==OUT && constante==OUT){
						printf("\nError: Secuencia de escape en la linea %d", linea);
						++error;
					}
					else if(s[i+1]!='n' && s[i+1]!='t' && s[i+1]!='b' && s[i+1]!='\"' && s[i+1]!='\\' && s[i+1]!='0'){
						printf("\nError: Secuencia de escape desconocida en la linea %d", linea);
						++error;
					}
				}

			else{
				if(s[i]=='\"'){ 
					if(constante==OUT)
						if(cadena==IN)
							cadena = OUT;
						else
							cadena = IN;
					else if(barra==OUT){
						printf("\nError: Comillas dentro de constante de caracter en la linea %d", linea);
						++error;
					}
				}

				else if(s[i]=='\'')
					if(cadena==IN){
						printf("\nError: Apostrofe dentro de cadena en la linea %d", linea);
						++error;
					}
					else if(constante==IN)
						constante = OUT;
					else{
						constante = IN;                 
						if(s[i+1]=='\''){
							printf("\nError: Constante de caracter vacio en la linea %d", linea);                                           ++error;
						}
						else if((s[i+1]!='\\' && s[i+2]!='\'' && s[i+2]!='\n') || (s[i+1]=='\\' && s[i+3]!='\'' && s[i+3]!='\n')){
							printf("\nError: Mas de un caracter en la constante de caracter en la linea %d", linea);
							++error;
						}
					}
			
				if(constante==OUT && cadena==OUT)	/* Aqui se comprueban las alineaciones */
					if(s[i]=='[')
						if(cor==IN){
							printf("\nError: Sobra \'[\' en la linea %d", linea);
							++error;
						}
						else
							cor = IN;
					
					else if(s[i]==']')
						if(cor==OUT){
							printf("\nError: Sobra \']\' en la linea %d", linea);
							++error;
						}
						else
							cor = OUT;
						
					else if(s[i]=='(' || s[i]==')'){        /* poner el controlador de par <0 */
						if(cor==IN){        
							printf("\nError: Falta \']\' antes del parentesi en la linea %d", linea);
							++error;
							cor = OUT;
						}
						if(s[i]=='(')
							++par;
						else if(par==0){
							printf("\nError: Falta \'(\' en la linea %d", linea);
							++error;
						}
						else
							--par;
					}

					else if(s[i]=='{' || s[i]=='}'){
						if(cor==IN){
							printf("\nError: Falta \']\' antes de la llave en la linea %d", linea);
							++error;
							cor = OUT;
						}
						if(par>0){
							for(j=0; j<par; ++j){
								printf("\nError: Falta \')\' antes de la llave en la linea %d", linea);
								++error;
							}
							par = 0;
						}
						if(s[i]=='{'){
							mlla[plla] = linea;
							++plla;
						}
						else if(plla==0){
							printf("\nError: Falta '{' para la llave de la linea %d", linea);
							++error;
						}
						else
							--plla;
					}

				barra = OUT;
			}
				
		++i; 
		
	}

	/* aqui se comprueba el programa despues de EOF */
	if(cadena==IN){
		printf("\nError: Falta \'\"\' en la linea %d", linea);
		++error;
	}
	else if(constante==IN){
		printf("\nError: Falta \'\'\' en la linea %d", linea);
		++error;
	}
	if(cor==IN){
		printf("\nError: Falta \']\' en la linea %d", linea);
		++error;
	}
	if(par>0){
		for(j=0; j<par; ++j){
			printf("\nError: Falta \')\' en la linea %d", linea);
			++error;
		}
		par = 0;
	}
	for(j=0; plla>j; ++j){
		printf("\nError: La llave de la linea %d no esta cerrada", mlla[j]);
		++error;
	}
	
	return i;
}