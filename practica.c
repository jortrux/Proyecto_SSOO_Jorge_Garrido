#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
	unsigned char ETQ;
	unsigned char Data[TAM_LINEA];
} T_LINEA_CACHE;

void limpiaCACHE(T_LINEA_CACHE tb1[NUM_FILAS]);
void VuelcaCACHE(T_LINEA_CACHE *tbl);
void parsearAddr(unsigned int addr; int *ETQ, int *palabra, int *linea, int *bloque);
void TratarFallo(T_LINEA_CACHE *tb1, char *MRAM, int ETQ, int linea, int bloque);

void main(int argc, char** argv){


}
