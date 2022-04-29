/* ejercicio 3_2 */

/* escape: convierte caracteres nueva linea y tabulacion
	en secuencias de escape visibles (\n y \t) mientras
	copia la cadena t a s */
void escape(char s[], char t[])
{
	int i=0, j=0;
	
	while((s[j]=t[i]) != '\0'){
		switch(t[i++]){
		case '\n':
			s[j++]='\\';
			s[j]='n';
			break;
		case '\t':
			s[j++]='\\';
			s[j]='t';
			break;
		}
		++j;
	}
}

/* inv_escape: convierte secuencias de escape nueva
	linea y tabulacion visibles (\n y \t) en caracteres
	mientras copia la cadena t a s */
void inv_escape(char s[], char t[])
{
	int i=0, j=0;

	while((s[j]=t[i]) != '\0'){
		if(t[i++] == '\\')
			switch(t[i]){
			case 'n':
				s[j]='\n';
				++i;
				break;
			case 't':
				s[j]='\t';
				++i;
				break;
			}
		++j;
	}
}