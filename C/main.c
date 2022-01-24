#ifndef INCLUDE
#define INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // for INT_MAX, INT_MIN
#include <time.h>

#include "cn.h"
#endif

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
			archivoPtr = fopen("notas", "r");
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
				archivoPtr = fopen("notas", "r");
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
		{
			char *p;
			long id = strtol(argv[2], &p, 10); // ver el puntero *p !!!!!!

			if (*p != '\0' || id > ULONG_MAX || id <= 0)
				puts("Error de parametro id");
			else
			{
				printf("Borrando la nota n° %lu.\n", id);
				FILE *archivoPtr;
				char buffer[255] = {};
				archivoPtr = fopen("notas", "r+");
				while ((fscanf(archivoPtr, "%[^\n]%*c", buffer)) != EOF)
				{
					/* PARECE QUE HABRIA QUE COPIAR TODO Y LUEGO HACER UN ARCHIVO NUEVO SIN LA LINEA DROPEAR*/
					// VER QUE HACE FSEEK
					// VER QUE HACE FTELL
					int totalBuffer = strlen(buffer);
					char *linea = (char *)malloc(totalBuffer);
					memcpy(linea, buffer, totalBuffer);
					char *campoId = strtok(buffer, " | ");
					if (atoi(campoId) == id)
					{
						printf("\n--->%lu\n",ftell(archivoPtr));
						// ftell me da el marcador del file.
						// en este caso es el final de la linea coincidente
						fseek(archivoPtr,-totalBuffer+6,SEEK_CUR);
						
						for (int x=0;x<totalBuffer-7;x++)
							fputc('\0',archivoPtr);
						fputc('\n',archivoPtr);

						fseek(archivoPtr,1,SEEK_CUR);

						//fputs("ESTO SERA BORRADO\n", archivoPtr);

						// ahora deberia imprimir un vacio en esa posicion.
						printf("\n--->%lu\n",ftell(archivoPtr));
						free(linea);
						fclose(archivoPtr);
						return 1;
					}
				}
			}
		}
	};

	puts("Opción incorrecta...Opción h para ayuda.");
}