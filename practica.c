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

int globaltime=0;

void main(int argc, char** argv){

        int numfallos=0;
	unsigned char Simul_Ram[4096];
	int i=0;
	int *addr;
	int ETQ, palabra, linea, bloque;
	char texto[100];

	T_LINEA_CACHE tbl[NUM_FILAS];
	FILE *fd;

	//lo marco porque nunca veo donde lo he escrito+++++++++++++++++++++++++++++
	limpiaCACHE(tbl);//paso a los valores por defectos al empezar la ejecuvion+|
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	fd=fopen("CONTENTS_RAM.bin","r");//abro contents ram

        if(fd==NULL){
                printf("Error, el archivo no existe\n");
        }else{
//                printf("el archivo existe\n");
		i=0;
                while(!feof(fd)){
//			printf("core\n");
                       	Simul_Ram[i]=fgetc(fd);//se me habia olvidado cambiar lo de parsear de aqui
			i++;
        	}

                fclose(fd);
		        //printf("%s\n",Simul_Ram);

        	fd=fopen("accesos_memoria.txt","r");//abro accesos a memoria
	        printf("%p",fd);
        	if(fd==NULL){
	                printf("Error, el archivo no existe\n");
        	}else{
	                i=0;
        	        int Hex;
//              	printf("el archivo existe\n");
                	while((fscanf(fd,"%X",&Hex)!=EOF)){
                	        parsearAddr(Hex,&ETQ,&palabra, &linea, &bloque);
                	        globaltime++;

                	        //fallos
                        	if(ETQ!=tbl[linea].ETQ){//COÑO en la linea no en cada iteracion, mea culpa
                                	numfallos++;
                                	printf("ha habido un fallo (%i)\n",numfallos);
					printf("T: %i, Fallo de CACHE %i, ADDR %X ETQ %X linea %X palabra %X bloque %X\n", globaltime, numfallos,Hex,ETQ,linea,palabra,bloque);
//                              	printf("no hay core\n");
                                	TratarFallo(tbl, Simul_Ram, ETQ,linea,bloque);
//                              	printf("si hay core\n");
                        	}
				printf("T: %i, Acierto de CACHE, ADDR %X ETQ %X linea %X palabra %X bloque %X DATO %X\n", globaltime, Hex, ETQ, linea, palabra, bloque, tbl[linea].Data[palabra]);
        	        	for(int i=0; i<NUM_FILAS; i++){//a cimprobar que coño estoy haciendo jejejej
	                                printf("ETQ:%X\tData ",tbl[i].ETQ);
                                        for(int j=0; j<TAM_LINEA; j++){
         	                               printf("%X ",tbl[i].Data[j]);
      	                                }
                                        printf("\n");
                                }
       	                        printf("\n\n");
               	        	texto[i]=tbl[linea].Data[palabra];
			        sleep(1);
				i++;
                	}
                	fclose(fd);
        	}
        	printf("Accesos totales: %i; fallos: %i; tiempo medio: %f\n",i, numfallos, (float)globaltime/i);
        	printf("Texto leido: %s\n\n",texto);
        	VuelcaCACHE(tbl);
        }
}


void limpiaCACHE(T_LINEA_CACHE tbl[NUM_FILAS]){
	printf("Cache limpia\n");
	for(int i=0; i<NUM_FILAS; i++){//recorro los arrays para poner los valores por defecto
		tbl[i].ETQ=0xFF;
		printf("ETQ:FF\tData ");
		for(int j=0; j<TAM_LINEA; j++){
			tbl[i].Data[j]=0x23;
			printf("%i ",tbl[i].Data[j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void VuelcaCACHE(T_LINEA_CACHE *tbl){
	printf("\n\nVolcado a cache\n\n");
	FILE *Ch;
	Ch=fopen("CONTENTS_CACHE.bin","w+");

	if(Ch==NULL){
		printf("error algo no va como debe\n");//redundante, si no existe se crea
	}else{
//		printf("archivo abierto\n");

		for(int i=0; i<NUM_FILAS; i++){//basicamente la funcion de limpiar cache, pero en lugar de limpiar guardo en un fichero
		//mi teoria se ha probado como cierta, el %c si que casteaba los valores a char, por eso ahora es un fprintf() en lugar de un fputc()
			printf("ETQ:%X\tData ",tbl[i].ETQ);
			//fputc(tbl[i].ETQ, Ch);
			fprintf(Ch,"ETQ:%X\tData ",tbl[i].ETQ);
			for(int j=0; j<TAM_LINEA; j++){
//        	                fputc(tbl[i].Data[j], Ch);
				fprintf(Ch,"%X ",tbl[i].Data[j]);
				printf("%X ",tbl[i].Data[j]);
	                }
		fputc('\n',Ch);
		printf("\n");
		}

		fclose(Ch);
	}
}

void parsearAddr(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque){
//https://nomadaselectronicos.wordpress.com/2015/03/05/manipulacion-de-bits-en-c-y-aplicaciones/
//https://www.tutorialspoint.com/cprogramming/c_operators.htm

//esto lo he plagiado porque no soy capaz de entenderlo por completo al 100%
//si me da tiempo lo hare de otra forma que tengo pensada, pero no se si podre
	*palabra = addr & 0b1111;
	*bloque = addr >> 4;
	*linea = (*bloque & 0b111);
	*ETQ = (*bloque & 0b11111000)>>3;
}

void TratarFallo(T_LINEA_CACHE *tbl, char *MRAM, int ETQ, int linea, int bloque){
	globaltime=globaltime+10;
	int inicio=0;
	inicio=bloque*16;
	tbl[linea].ETQ=ETQ;
	for(int i=0; i<16; i++){//16=tam bloque
		tbl[linea].Data[i]=MRAM[i+inicio];
	}
}
