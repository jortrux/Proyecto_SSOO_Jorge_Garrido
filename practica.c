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
	int i=0;
	FILE *fd;


	fd=fopen("CONTENTS_RAM.bin","r");
        printf("%p",fd);
        if(fd==NULL){
                printf("Error, el archivo no existe");
        }else{
                printf("el archivo existe");


                while(!feof(fd)){
                       	Simul_Ram[i]= fgetc(fd);
			i++;
        	}

                fclose(fd);
        }


	printf("%s",Simul_Ram);


	fd=fopen("accesos_memoria.txt","r");
	printf("%p",fd);
	if(fd==NULL){
		printf("Error, el archivo no existe");
	}else{

		printf("el archivo existe");

		fclose(fd);
	}
}
