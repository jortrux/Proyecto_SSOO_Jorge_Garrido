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
	int *addr;
	int ETQ, palabra, linea, bloque;

	T_LINEA_CACHE tbl[NUM_FILAS];
	FILE *fd;

	//lo marco porque nunca veo donde lo he escrito+++++++++++++++++++++++++++++
	limpiaCACHE(tbl);//paso a los valores por defectos al empezar la ejecuvion+|
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	fd=fopen("CONTENTS_RAM.bin","r");//abro contents ram

        if(fd==NULL){
                printf("Error, el archivo no existe\n");
        }else{
                printf("el archivo existe\n");

                while(!feof(fd)){
                       	Simul_Ram[i]= fgetc(fd);
			i++;

			parsearAddr(addr[i],&ETQ,&palabra, &linea, &bloque);
        	}

                fclose(fd);
        }

	//printf("%s\n",Simul_Ram);

	fd=fopen("accesos_memoria.txt","r");//abro accesos a memoria
	printf("%p",fd);
	if(fd==NULL){
		printf("Error, el archivo no existe\n");
	}else{
		int Hex;
		printf("el archivo existe\n");
		while((fscanf(fd,"%X",&Hex)!=EOF)){
                        parsearAddr(Hex,&ETQ,&palabra, &linea, &bloque);
			glovaltime++;

			//fallos
                	if(ETQ!=tbl[i].ETQ){
                	        numfallos++;
        	                TratarFallo(tbl, Simul_Ram, ETQ,linea,bloque);
	                }
		i++;
                }
                fclose(fd);
	}
}


void limpiaCACHE(T_LINEA_CACHE tbl[NUM_FILAS]){

	for(int i=0; i<NUM_FILAS; i++){//recorro los arrays para poner los valores por defecto
		tbl[i].ETQ=0xFF;
		printf("ETQ:FF\tData ");
		for(int j=0; j<TAM_LINEA; j++){
			tbl[i].Data[j]=0x23;
			printf("%i ",tbl[i].Data[j]);
		}
		printf("\n");
	}
}

void VuelcaCACHE(T_LINEA_CACHE *tbl){
	FILE *Ch;
	Ch=fopen("CONTENTS_CACHE.bin","w+");

	if(Ch==NULL){
		printf("error algo no va como debe\n");//redundante, si no existe se crea
	}else{
		printf("archivo abierto\n");

		for(int i=0; i<NUM_FILAS; i++){
			fputc(tbl[i].ETQ, Ch);
			fputc('\n',Ch);

			for(int j=0; j<TAM_LINEA; j++){
        	                fputc(tbl[i].Data[j], Ch);
				fputc('\n', Ch);
	                }
		}

		fclose(Ch);
	}
}

void parsearAddr(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque){
//https://nomadaselectronicos.wordpress.com/2015/03/05/manipulacion-de-bits-en-c-y-aplicaciones/
//https://www.tutorialspoint.com/cprogramming/c_operators.htm

//esto lo he plagiado porque no soy capaz de entenderlo por completo al 100%
	*palabra = addr & 0b1111;
	*bloque = addr >> 4;
	*linea = (*bloque & 0b111);
	*ETQ = (*bloque & 0b11111000)>>3;
}

void TratarFallo(T_LINEA_CACHE *tbl, char *MRAM, int ETQ, int linea, int bloque);
