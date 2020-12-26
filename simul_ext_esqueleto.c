#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"
#include<stdlib.h>

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main()
{
	 char comando[LONGITUD_COMANDO];
         int f_comando;
	 int i,j;
	 unsigned long int m;
     EXT_SIMPLE_SUPERBLOCK ext_superblock;
      //Variables que necesitamos cambiar
       //s_free_blocks_count;
       //s_free_inodes_count;
     EXT_BYTE_MAPS ext_bytemaps;
      //Todas son cambiadas (-relleno):
       //bmap_inodos;
       //bmap_bloques;
     EXT_BLQ_INODOS ext_blq_inodos;
      //Cambiar todo menos relleno
     EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
      //Cambiar solo dir_nfich[] en caso de que no tengamos que crear directorios;
     EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
     EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
     int entradadir;
     int grabardatos;
     FILE *fent;
     
     // Lectura del fichero completo de una sola vez
     
     fent = fopen("particion.bin","r+b");
     if(fent == NULL){
        exit(0);
     }
     fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
     
     //Inicializacion variables
     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
     memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);


     do{
	printf("\n>> ");
	fgets(comando, LONGITUD_COMANDO, stdin);
	f_comando = ComprobarComando(comando);
	switch(f_comando){
		case 0: //Salir
		break;
		case 1: //Info
		break;
		case 2: //bytemaps
		break;
		case 3: //dir
		break;
		case 4: //rename
		break;
		case 5: //imprimir
		break;
		case 6: //remove
		break;
		case 7: //copy
		break;
		case 8: //ERROR
			printf("ERROR: Comando ilegal [bytemaps,copy,dir,info,imprimir,rename,remove,salir]");
		break;
	}
    }while(f_comando!=0);
     // Buce de tratamiento de comandos
     //for (;;){
//		 do {
//		 printf (">> ");
//		 fflush(stdin);
//		 fgets(comando, LONGITUD_COMANDO, stdin);
//		 } while (ComprobarComando(comando,orden,argumento1,argumento2) !=0);
//	     if (strcmp(orden,"dir")==0) {
//            Directorio(&directorio,&ext_blq_inodos);
//            continue;
//            }
         // Escritura de metadatos en comandos rename, remove, copy
//         Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
//         GrabarByteMaps(&ext_bytemaps,fent);
//         GrabarSuperBloque(&ext_superblock,fent);
//         if (grabardatos)
//           GrabarDatos(&memdatos,fent);
//         grabardatos = 0;
         //Si el comando es salir se habrán escrito todos los metadatos
         //faltan los datos y cerrar
//         if (strcmp(orden,"salir")==0){
//            GrabarDatos(&memdatos,fent);
//            fclose(fent);
//            return 0;
//         }
//     }
}

int ComprobarComando(char *strcomando){
	char string[LONGITUD_COMANDO];
	sscanf(strcomando, "%s", string);
	if(strcmp(string, "info") == 0){
		return 1;
	} else if(strcmp(string, "bytemaps") == 0){
                return 2;
        } else if(strcmp(string, "dir") == 0){
                return 3;
        } else if(strcmp(string, "rename") == 0){
                return 4;
        } else if(strcmp(string, "imprimir") == 0){
                return 5;
        } else if(strcmp(string, "remove") == 0){
                return 6;
        } else if(strcmp(string, "copy") == 0){
                return 7;
        } else if(strcmp(string, "salir") == 0){
                return 0;
        } else {
                return 8;
        }
}

//REVIEW
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
   printf("Bloque %d Bytes\n", psup.s_block_size);
   printf("inodos particion = %d\n", psup.s_inodes_count);
   printf("inodos libres = %d\n", psup.s_free_inodes_count);
   printf("Bloques particion = %d\n", psup.s_blocks_count);
   printf("Bloques libres = %d\n", psup.s_free_blocks_count);
   printf("Primer bloque de datos = %d\n", psup.s_first_data_block);
}

//REVIEW
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){
   //Comienza en el 1 porque en el 0 está la entrada especial '.' que no necesitamos mostrar
   for(int i=1; i<MAX_FICHEROS; i++){
      if(directorio[i].dir_nfich != "")
         printf("%s /t tamanio: %d /t inodo: %d bloques: %d\n", directorio[i].dir_nfich, inodos.bql_inodos[directorio[i].dir_inodo].size_fichero, directorio[i].dir_inodo, inodos.bql_inodos[directorio[i].dir_inodo].i_nbloque);
   }
}

//Esta función imprime los bytemaps
void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
        int i;
        printf("Inodos:");
        //i empieza en la posición 0 y llega hasta MAX_INODOS -1, va imprimiendo el
        //número/carácter en cada posición con espacios.
        for(i=0; i < MAX_INODOS; i++){
                printf(" %c", ext_bytemaps->bmap_inodos[i]);
        }
        //Lo mismo que arriba pero con el bytemap de bloques.
        printf("\nBloques:");
        for(i=0; i < MAX_BLOQUES_PARTICION; i++){
                printf(" %c", ext_bytemaps->bmap_bloques[i]);
        }
        printf("\n");
}