#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#define MAXESC 20 /* máximo número de veces que aplicaremos */
#define ESCDEF 5 /* "box-counting" - por defecto, 5 veces - */
#define MINPTOS 10 /* mínimo número de puntos que debe tener y(x) */
#define SI 1
#define NO 0
#define x0DEF -1.0
#define x1DEF 1.0

/* Roger Torrent
   Fractales en la Red
   Curso 2002-2003 (Versión 2009)

   Programa calculador de funciones Carotid-Kundalini [y(x) = cos(n·x·acos(x))]
   y de su dimensión fractal.
   
   Ejecución desde el DOS:
      ck n dx [/i:x0,x1] [/d[:esc,min]] [/t] [/a] > archivo_de_salida
   donde
      n -> primeras 'n' curvas
	  dx -> el paso a lo largo del eje x
	  /i:x0,x1 -> [opcional] intervalo de x (por defecto, [x0DEF,x1DEF])
	  /d:esc,min -> [opcional] cálculo de la dimensión fractal por el
	       método de "box-counting", donde esc (<= MAXESC) representa el
		   número de escalas sobre los que contaremos y min (>= dx)
		   el tamaño de la escala más pequeña (por defecto, esc = ESCDEF y min = dx)  
	  /t -> [opcional] visualización del % completado y del tiempo restante
	  /a -> [opcional] aviso acústico al terminar
   Prescindiendo de "> archivo_de_salida", la salida es por pantalla. Los argumentos
   opcionales no pueden combinarse.

   Formato del archivo_de_salida (por filas):
      x y1(x) y2(x) ... yn(x)
   Si la opción 'd' fue activada, sólo se graban los resultados del "box-counting": 
	  ln(1/r1) ln(N(r))
	  ln(1/r2) ln(N(r))
	  ...
	  ln(1/min) ln(N(min))
	  Dimension fractal = #.### */

void main(int argc, char *argv[])
{
	int i, j, n, dim = NO, esc = ESCDEF, tcontrol = NO, aviso = NO;
	double x, y ,dx, min;
	double x0 = x0DEF, x1 = x1DEF; /* valores por defecto */
    char *llamada = argv[0];
	unsigned long Nr[MAXESC], numbytes[MAXESC], totalbytes = 0;
	unsigned long indicex, indicey, pasosx[MAXESC], pasosy[MAXESC];
	double r[MAXESC], log1r[MAXESC], logNr[MAXESC], mlog1r = 0, mlogNr = 0;
	char *cajas[MAXESC], punto, *pcaja;
	float xf; /* fracción de x completada */
	int seg = 0, fin; /* segundos cumplidos y restantes */
	double Num = 0, Den = 0; /* pendiente de la recta de regresión */

	/* inicialización: entrada de valores y opciones del usuario */
	if(argc < 3)
		goto error;
	n = atoi(argv[1]);
	dx = min = atof(argv[2]);
	if(n <= 0 || dx <= 0){
		fprintf(stderr, "Error: \'n\' o \'dx\' <= 0\n");
		exit(1);
	}
	argc -= 2;
	argv += 2;
	while(--argc > 0 && (*++argv)[0] == '/')
		switch(i = *++argv[0]){
			case 'i': /* intervalos definidos por el usuario */
				*argv += 2;
				x0 = atof(strtok(argv[0],","));
				x1 = atof(strtok(NULL, ""));
				if((x1 <= x0) || (x0 < -1) || (x1 > 1)){
					fprintf(stderr, "Error: no se cumple -1 <= x0 < x1 <= +1\n");
					exit(1);
				}
				break;
			case 'd': /* calculará dimensiones */
				dim = SI;
				if(*++argv[0] == ':'){
					(*argv)++;      
					esc = atoi(strtok(argv[0],","));
					if(esc > MAXESC || esc < 1){
						fprintf(stderr, "Error: \'esc\' debe quedar entre 1 y %d\n", MAXESC);
						exit(1);
					}
					min = atof(strtok(NULL, ""));
					if(min > (y = (x1-x0)/MINPTOS) || min < dx){
						fprintf(stderr, "Error: \'min\' debe quedar entre \'dx\' y %g\n", y);
						exit(1);
					}
				}
				break;
			case 't': /* control del tiempo de ejecución */
				tcontrol = SI;
				break;
			case 'a': /* aviso acústico de fin de programa */
				aviso = SI;
				break;
			default:
				goto error;
		}
	if(argc)
		goto error;
	/* comprobación del número de puntos de interpolación de y(x) */
	if((unsigned long)floor((x1-x0)/dx) + 1 < MINPTOS){
		fprintf(stderr, "Error: Insuficientes puntos de calculo; disminuya \'dx\'\n");
		exit(1);
	}
	
	if(dim){
	/* comprobación del número de cajas */
		if(ceil((x1-x0)/min)*ceil(2/min) > ULONG_MAX){
			fprintf(stderr, "Error: Numero de cajas excesivo; aumente \'esc\' o disminuya el intervalo\n");
			exit(1);
		}
	/* comprobación de la densidad de trazos en la última escala */
		x = (2/min);
		if(n < x){
			fprintf(stderr, "Error: Insuficientes curvas en la ultima escala; aumente \'n\' o \'min\'\n");
			exit(1);
		}
	/* cálculo de escalas; buscamos ln(1/r) equiespaciados con r[0] = (x1-x0)/MINPTOS y r[esc-1] = min */
		r[esc-1] = min;
		for(i = esc-2; i >= 0; i--)
			r[i] = min*pow((x1-x0)/(min*MINPTOS), 1-(double)i/(esc-1));
	/* asignación de memoria e inicialización de las cajas a cero (formato: secuencia de bytes,
		donde cada byte representa 8 posiciones a lo largo del eje 'x' y una en 'y'
		-> complicado, pero ahorra mucha memoria) */
		for(i = 0; i < esc; i++){
			pasosx[i] = (unsigned long)ceil((x1-x0)/r[i]);
			pasosy[i] = (unsigned long)ceil(2/r[i]);
			numbytes[i] = (unsigned long)ceil((double)pasosx[i]/8) * pasosy[i];
			totalbytes += numbytes[i];
			fprintf(stderr, "Escala %d: %.4e -> %u bytes (%.2f MBytes)\n", i+1, r[i], numbytes[i], (float)numbytes[i]/1048576);
			if((cajas[i] = (char *)calloc(numbytes[i], 1)) == NULL){
				fprintf(stderr, "Error: Memoria insuficiente para gestionar la escala %d\n", i+1);
				while(--i >= 0)
					free(cajas[i]);
				exit(1);
			}
			Nr[i] = 0; /* contador a cero */
		}
		fprintf(stderr, "Total: %.2f MBytes\n", (float)totalbytes/1048576);
	}
	/* bucle principal */
	for(x = x0; x < x1+dx; x += dx){
		if(x > x1)
			x = x1;
		if(tcontrol && (i = clock()/CLK_TCK) > seg){ /* contador de segundos */
			seg = i;
			xf = (float)((x-x0)/(x1-x0));
			fprintf(stderr, "\rProcesando... %2d%% ", (int)floor(xf*100));
			fin = (int)ceil((1-xf)/xf*seg); /* segundos que quedan */
			fprintf(stderr, "Duracion estimada: ");
			if(i = fin/60)
				fprintf(stderr, "%dmin ", i);
			fprintf(stderr, "%dseg\t\t", fin%60);
		}
		if(!dim)
			printf("%g ", x);
		for(i = 1; i <= n; i++){
			y = cos(i * x * acos(x)); /* cálculo de y(x) */
			if(!dim) /* impresión de la función C-K */
				printf("%g ",y);
			else /* relleno de cajas */
				for(j = 0; j < esc; j++){ /* se indexa a partir del punto (x0,-1) */
					indicex = (unsigned long)floor((x - x0) / r[j]);
					if(indicex == pasosx[j])
						indicex--; /* por si justo coinciden x y x1 en el borde de una caja
								      -> caja fuera de rango */
					indicey = (unsigned long)floor((y + 1) / r[j]);
					if(indicey == pasosy[j])
						indicey--; /* por si justo coinciden y y +1 en el borde de una caja
								      -> caja fuera de rango */
					punto = 128>>indicex%8;
					pcaja = cajas[j]+pasosy[j]*(indicex/8)+indicey;
					if(!(*pcaja & punto)){
						*pcaja |= punto;
						Nr[j]++;
					}
				}
		}
		if(!dim)
			printf("\n");
	}
	if(tcontrol){
		seg = clock()/CLK_TCK;
		fprintf(stderr, "\rCompletado en ");
		if(i = seg/60)
			fprintf(stderr, "%dmin ", i);
		fprintf(stderr, "%dseg\t\t\t\t\t\n", seg%60);
	}
	if(dim){ /* presentación del "box-counting" en escala log-log */
		for(i = 0; i < esc; i++){
			log1r[i] = -log(r[i]);
			mlog1r += log1r[i];
			logNr[i] = log(Nr[i]);
			mlogNr += logNr[i];
			printf("%g %g\n", log1r[i], logNr[i]);
			free(cajas[i]);	/* libera la memoria */ 
		}
		mlog1r /= esc; /* valores medios */
		mlogNr /= esc;
		for(i = 0; i < esc; i++){ /* pendiente de la recta de regresión */
			Num += (x = (log1r[i]-mlog1r))*(logNr[i]-mlogNr);
			Den += x * x;
		}
		printf("Dimension fractal = %.3f", (float)(Num/Den));
	}
	if(aviso) /* bocina */
		fprintf(stderr, "\a");
	return; /* fin de programa */

error:
	fprintf(stderr,"Sintaxis: %s n dx [/i:x0,x1] [/d[:esc,min]] [/t] [/a]\n", llamada);
	exit(1);
}