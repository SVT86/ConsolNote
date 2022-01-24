#define INCLUDE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // for INT_MAX, INT_MIN
#include <time.h>

#include "cn.h"


/* AYUDA ***********************************************************************************/
int ayuda()
{
	const char *items[N_ITEM] = {
		"w: escribe nota",
		"r: lee notas",
		"d: borra nota",
		"h: ayuda",
		"v: muestra versión"};

	for (int i = 0; i < N_ITEM; i++)
		printf("%s.\n", items[i]);

	return 1;
}
/* ESCRIBIR ********************************************************************************/
int escribir(const char *argumento)
{
	registro entrada;
	char mensaje[144] = "";
	FILE *archivoPtr;

	if (!argumento)
		strcpy(mensaje, "");
	else
		memcpy(mensaje, argumento, 143); // copia byte a byte

	time_t t = time(NULL);

	struct tm tiempoLocal = *localtime(&t);

	const char *formato = "%d-%m-%Y";
	//printf("\n| \e[1;36m%i\e[m | \e[1;31m%s\e[m | \e[0;34m %s \e[m |\n\n", entrada.id, entrada.date, entrada.contenido);

	// averiguar cuantas lineas para el proximo id.
	archivoPtr = fopen("notas", "a+");
	long int contador = 1;
	if (archivoPtr)
	{
		char ch;
		do
		{
			ch = fgetc(archivoPtr);
			if (ch == '\n')
				contador++;

		} while (ch != EOF);

		printf("Total de entradas registradas: %lu\n", contador);
	}
	else
	{
		puts("ERROR");
		return 0;
	}
	char buffer[255];
	char *id;

	entrada.id = contador;

	strftime(entrada.date, sizeof entrada.date, formato, &tiempoLocal);
	strcpy(entrada.contenido, mensaje);
	sprintf(buffer, "%lu", contador);
	strcat(buffer, " | ");
	strcat(buffer, entrada.date);
	strcat(buffer, " | ");
	strcat(buffer, entrada.contenido);
	strcat(buffer, "\n");
	fputs(buffer, archivoPtr);

	fclose(archivoPtr);

	return 1;
}


