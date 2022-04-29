/* ejercicio 2_1 */
/* determina los rangos de las variables */
/* constantes sacadas del Apendice B del libro */

#include <stdio.h>
#include <limits.h>
#include <float.h>

main()
{
	char cmin, cmax, temp1;
	signed char scmin, scmax, temp2;
	unsigned char ucmax, temp3;
	int imin, imax, temp4;
	unsigned int uimax, temp5;
	long int lmin, lmax, temp6;
	unsigned long int ulmax, temp7;
	short int hmin, hmax, temp8;
	unsigned short int uhmax, temp9;
	
	printf("Rangos segun los headers estandar <limits.h> y <float.h> [por calculo directo]:");
	printf("\nEsto puede tardar un poco...");
	
	/* los calculos para los char, int, short y long son similares */
	/* se basan en el hecho de que el numero inferior y el superior son consecutivos */
	cmin = temp1;
	while(temp1 <= cmin){
		cmin = temp1;
		--temp1;
	}
	printf("\n\nValor minimo de \"char\": %d [%d]", CHAR_MIN, cmin);
	cmax = temp1;
	while(temp1 >= cmax){
		cmax = temp1;
		++temp1;
	}
	printf("\nValor maximo de \"char\": %d [%d]", CHAR_MAX, cmax);
	scmin = temp2;
	while(temp2 <= scmin){
		scmin = temp2;
		--temp2;
	}
	printf("\nValor minimo de \"signed char\": %d [%d]", SCHAR_MIN, scmin); 
	scmax = temp2;
	while(temp2 >= scmax){
		scmax = temp2;
		++temp2;
	}
	printf("\nValor maximo de \"signed char\": %d [%d]", SCHAR_MAX, scmax);
	ucmax = temp3;
	while(temp3 >= ucmax){
		ucmax = temp3;
		++temp3;
	}
	printf("\nValor maximo de \"unsigned char\": %u [%u]", UCHAR_MAX, ucmax);
	imin = temp4 = 0;
	while(temp4 <= imin){
		imin = temp4;
		--temp4;
	}
	printf("\n\nValor minimo de \"int\": %d [%d]", INT_MIN, imin); 
	imax = temp4;
	while(temp4 >= imax){
		imax = temp4;
		++temp4;
	}
	printf("\nValor maximo de \"int\": %d [%d]", INT_MAX, imax);
	uimax = temp5;
	while(temp5 >= uimax){
		uimax = temp5;
		++temp5;
	}
	printf("\nValor maximo de \"unsigned int\": %u [%u]", UINT_MAX, uimax);	
	lmin = temp6;
	while(temp6 <= lmin){
		lmin = temp6;
		--temp6;
	}
	printf("\n\nValor minimo de \"long\": %ld [%ld]", LONG_MIN, lmin);
	lmax = temp6;
	while(temp6 >= lmax){
		lmax = temp6;
		++temp6;
	}
	printf("\nValor maximo de \"long\": %ld [%ld]", LONG_MAX, lmax);	
	ulmax = temp7;
	while(temp7 >= ulmax){
		ulmax = temp7;
		++temp7;
	}
	printf("\nValor maximo de \"unsigned long\": %lu [%lu]", ULONG_MAX, ulmax);	
	hmin = temp8;
	while(temp8 <= hmin){
		hmin = temp8;
		--temp8;
	}
	printf("\n\nValor minimo de \"short\": %hd [%hd]", SHRT_MIN, hmin);	
	hmax = temp8;
	while(temp8 >= hmax){
		hmax = temp8;
		++temp8;
	}
	printf("\nValor maximo de \"short\": %hd [%hd]", SHRT_MAX, hmax);		
	uhmax = temp9;
	while(temp9 >= uhmax){
		uhmax = temp9;
		++temp9;
	}
	printf("\nValor maximo de \"unsigned short\": %hu [%hu]", USHRT_MAX, uhmax);
	
	/* evidentemente, los rangos de tipo float no se calculan directamente */
	printf("\n\nValor minimo de \"float\": %.9e", FLT_MIN);
	printf("\nValor maximo de \"float\": %.9e", FLT_MAX);	
	printf("\n\nValor minimo de \"double\": %.16e", DBL_MIN);
	printf("\nValor maximo de \"double\": %.16e", DBL_MAX);	
	printf("\n\nValor minimo de \"long double\": %.16Le", LDBL_MIN);
	printf("\nValor maximo de \"long double\": %.16Le", LDBL_MAX);

	return 0;
}