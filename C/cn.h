#ifndef CN
#define CN

#define N_ITEM 5

typedef struct
{
	int id;
	char date[25];
	char contenido[144];
} registro;

int ayuda(void);

int escribir(const char *argumento);

int lectura(char *arg2);

int borrar(char *argumento);

#endif