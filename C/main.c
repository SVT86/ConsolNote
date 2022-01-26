#ifndef INCLUDE
#define INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // for INT_MAX, INT_MIN
#include <time.h>

#include "cn.h"
#endif

/* MAIN ***********************************************************************************/
int main(int argc, char **argv)
{

	/* falta argumento de opción */
	if (!argv[1])
	{
		puts("Falta opción.Opción h para ayuda.");
		return 0;
	}

	/* muestra versión */
	if (*argv[1] == 'v')
	{
		puts("ConsolNote\nVersión 0.1");
		return 1;
	}

	/* muestra ayuda */
	if (*argv[1] == 'h')
		return ayuda();

	/* Escribir, Leer, Borrar */
	if (*argv[1] == 'w' || *argv[1] == 'r' || *argv[1] == 'd')
	{

		/* Escribir */
		if (*argv[1] == 'w')
			return escribir(argv[2]);

		/* Leer */
		if (*argv[1] == 'r')
			return lectura(argv[2]);

		/* Borrar */
		if (*argv[1] == 'd' && argv[2])
			return borrar(argv[2]);
	};

	puts("Opción incorrecta...Opción h para ayuda.");
}