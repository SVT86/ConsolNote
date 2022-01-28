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
		"\e[0;32mw\e[m: escribe nota.\n\tw \"NOTA A GUARDAR\"",
		"\e[0;32mr\e[m: lee notas.\n\tr a : lee todas las notas.\n\tr n : lee la n nota",
		"\e[0;32md\e[m: borra nota\n\td n : borra la n nota.",
		"\e[0;32mh\e[m: ayuda",
		"\e[0;32mv\e[m: muestra versión"};

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

	// averiguar cuantas lineas para el proximo id.
	archivoPtr = fopen(".notas", "a+");
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

	entrada.id = contador;

	strftime(entrada.date, sizeof entrada.date, formato, &tiempoLocal);
	strcpy(entrada.contenido, mensaje);
	sprintf(buffer, "%4lu", contador);
	strcat(buffer, " | ");
	strcat(buffer, entrada.date);
	strcat(buffer, " | ");
	strcat(buffer, entrada.contenido);
	strcat(buffer, "\n");
	fputs(buffer, archivoPtr);

	fclose(archivoPtr);

	return 1;
}

/* LECTURA*********************************************************************************/
int lectura(char *arg2)
{
	if (arg2)
	{
		if (*arg2 == 'a')
		{
			FILE *archivoPtr;
			if (!(archivoPtr = fopen(".notas", "r")))
			{
				puts("\nBlock de notas no encontrado.\n");
				return 0;
			}
			char buffer[] = {};
			while ((fscanf(archivoPtr, "%[^\n]%*c", buffer)) != EOF)
				printf("\e[0;36m%s\n", buffer);
			fclose(archivoPtr);
			return 1;
		}

		if (*arg2 && *arg2 != 'a')
		{

			char buffer[255] = {};
			char *p;

			long id = strtol(arg2, &p, 10); // ver el puntero *p !!!!!!
			if (*p != '\0' || id > ULONG_MAX || id <= 0)
			{
				puts("Error de parametro id!!!");
				return 1;
			}
			else
			{
				FILE *archivoPtr;
				archivoPtr = fopen(".notas", "r");
				while ((fscanf(archivoPtr, "%[^\n]%*c", buffer)) != EOF)
				{
					int totalBuffer = strlen(buffer);
					char *linea = (char *)malloc(totalBuffer);
					memcpy(linea, buffer, totalBuffer);
					char *campoId = strtok(buffer, " | ");
					if (atoi(campoId) == atoi(arg2))
					{

						printf("\n\e[0;36m%s\n\n", linea);
						free(linea);
						fclose(archivoPtr);
						return 1;
					}
				}

				printf("\e[0;31m id \e[4m%lu\e[0;31m not found.\e[0m\n", id);
				return 0;
			}

			return 1;
		}
	}
	puts("Especifique opción de lectura.");
	return 0;
}

/* BORRADO*********************************************************************************/
int borrar(char *argumento)
{

	char *p;
	long id = strtol(argumento, &p, 10); // ver el puntero *p !!!!!!

	if (*p != '\0' || id > ULONG_MAX || id <= 0)
		puts("Error de parametro id");
	else
	{
		FILE *archivoPtr;
		char buffer[255] = {};
		if (!(archivoPtr = fopen(".notas", "r+")))
		{

			puts("\nBlock de notas no encontrado.\n");
			return 0;
		}
		while ((fscanf(archivoPtr, "%[^\n]%*c", buffer)) != EOF)
		{
			int totalBuffer = strlen(buffer);
			char *linea = (char *)malloc(totalBuffer);
			memcpy(linea, buffer, totalBuffer);
			char *campoId = strtok(buffer, " | ");
			if (atoi(campoId) == id)
			{
				if (linea[7] == '\0')
				{
					printf("La entrada n° %lu ya ha sido borrada previamente.\n", id);
					free(linea);
					fclose(archivoPtr);
					return 1;
				}
				else
				{

					printf("Borrando la nota n° %lu.\n", id);
					// me posiciono cuando comienza la fecha.
					fseek(archivoPtr, -totalBuffer + 6, SEEK_CUR);

					// ahora 0 seria comienzo de fecha.
					// tengo que terminar 7 antes, sino me paso del final de la linea
					for (int x = 0; x < totalBuffer - 7; x++)
						fputc('\0', archivoPtr); // coloco tantos char vacios
					fputc('\n', archivoPtr);	 // y al final \n para que pueda contar despues.

					fseek(archivoPtr, 1, SEEK_CUR); // me adelanto 1 mas y caigo a la sig. linea.

					free(linea);
					fclose(archivoPtr);
					return 1;
				}
			}
		}

		printf("\e[0;31m id \e[4m%lu\e[0;31m not found.\e[0m\n", id);
		return 0;
	}
	return 0;
}