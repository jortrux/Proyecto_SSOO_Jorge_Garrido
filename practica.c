#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_FILAS 8
#define TAM_LINEA 16

typedef struct {
	unsigned char ETQ;
	unsigned char Data[TAM_LINEA];
} T_LINEA_CACHE;

void limpiaCACHE(T_LINEA_CACHE tbl[NUM_FILAS]);
void VuelcaCACHE(T_LINEA_CACHE *tbl);
void parsearAddr(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque);
void TratarFallo(T_LINEA_CACHE *tbl, char *MRAM, int ETQ, int linea, int bloque);

void main(int argc, char** argv){
	int glovaltime=0;
	int numfallos=0;
	unsigned char Simul_Ram[4096];
	//unsigned char **mem;
	int i=0;
	int ETQ, palabra, linea, bloque;
	unsigned int *addr;

	T_LINEA_CACHE tbl[NUM_FILAS];
	FILE *fd;

	limpiaCACHE(tbl);//paso a los valores por defectos al empezar la ejecuvion

	fd=fopen("CONTENTS_RAM.bin","r");//abro contents ram

        if(fd==NULL){
                printf("Error, el archivo no existe\n");
        }else{
                printf("el archivo existe\n");

                while(!feof(fd)){
                       	Simul_Ram[i]= fgetc(fd);
			i++;
        	}

                fclose(fd);
        }

	//printf("%s\n",Simul_Ram);

	fd=fopen("accesos_memoria.txt","r");//abro accesos a memoria
	printf("%p",fd);
	if(fd==NULL){
		printf("Error, el archivo no existe");
	}else{

		printf("el archivo existe");

		fclose(fd);
	}
}


void limpiaCACHE(T_LINEA_CACHE tbl[NUM_FILAS]){

	for(int i=0; i<NUM_FILAS; i++){//recorro los arrays para poner los valores por defecto
		tbl[i].ETQ=0xFF;
		for(int j=0; j<TAM_LINEA; j++){
			tbl[i].Data[j]=0x23;
		}
	}
}

void parsearAddr(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque){
//https://nomadaselectronicos.wordpress.com/2015/03/05/manipulacion-de-bits-en-c-y-aplicaciones/
//https://www.tutorialspoint.com/cprogramming/c_operators.htm

//esto lo he plagiado porque no soy capaz de entenderlo por completo al 100%
	*palabra = addr & 4;
	*bloque = addr >> 4;
	*linea = (*bloque & 0b111);
	*ETQ = (*bloque & 0b11111000)>>3;
}


