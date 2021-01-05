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
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps);
int Renombrar(EXT_ENTRADA_DIR *directorio,EXT_BYTE_MAPS *ext_bytemaps ,char *comando);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre, EXT_BYTE_MAPS *bytemaps);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *comando);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, FILE *fich, char *comando);
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
			LeeSuperBloque(&ext_superblock);
		break;
		case 2: //bytemaps
			Printbytemaps(&ext_bytemaps);
		break;
		case 3: //dir
			Directorio(directorio,&ext_blq_inodos,&ext_bytemaps);
		break;
		case 4: //rename
			Renombrar(directorio, &ext_bytemaps, comando);
		break;
		case 5: //imprimir
                        Imprimir(directorio, &ext_blq_inodos, memdatos, comando, &ext_bytemaps);
		break;
		case 6: //remove
                        Borrar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, comando);
		break;
		case 7: //copy
			Copiar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, memdatos, fent, comando);
		break;
		case 8: //ERROR
			printf("ERROR: Comando ilegal [bytemaps,copy,dir,info,imprimir,rename,remove,salir]");
		break;
	}
    }while(f_comando!=0);
			memcpy((EXT_SIMPLE_SUPERBLOCK *)&datosfich[0],&ext_superblock, SIZE_BLOQUE);
                        memcpy((EXT_ENTRADA_DIR *)&datosfich[3],&directorio, SIZE_BLOQUE);
                        memcpy((EXT_BLQ_INODOS *)&datosfich[1],&ext_bytemaps, SIZE_BLOQUE);
                        memcpy((EXT_BLQ_INODOS *)&datosfich[2],&ext_blq_inodos, SIZE_BLOQUE);
                        memcpy((EXT_DATOS *)&datosfich[4],&memdatos, MAX_BLOQUES_DATOS*SIZE_BLOQUE);
                        rewind(fent);
			for(i = 0; i<MAX_BLOQUES_PARTICION;i++){
                                for(j = 0; j<SIZE_BLOQUE; j++){
                                        fputc(datosfich[i].dato[j],fent);
                                }

    }
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

//Imprime por pantalla la informacion del superbloque
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
   printf("Bloque %d Bytes\n", psup->s_block_size);
   printf("inodos particion = %d\n", psup->s_inodes_count);
   printf("inodos libres = %d\n", psup->s_free_inodes_count);
   printf("Bloques particion = %d\n", psup->s_blocks_count);
   printf("Bloques libres = %d\n", psup->s_free_blocks_count);
   printf("Primer bloque de datos = %d\n", psup->s_first_data_block);
}

//Muestra la información del directorio
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps){
   //Comienza en el 1 porque en el 0 está la entrada especial '.' que no necesitamos mostrar
   for(int i=1; i<MAX_FICHEROS; i++){
      //Comprobamos que el inodo existe y está ocupado
      if(ext_bytemaps->bmap_inodos[directorio[i].dir_inodo] == 1){
         printf("%s \t tamanio: %d \t inodo: %d bloques:", directorio[i].dir_nfich, inodos->blq_inodos[directorio[i].dir_inodo].size_fichero, directorio[i].dir_inodo);
         for(int j=0; j<MAX_NUMS_BLOQUE_INODO; j++){
            //Comprobamos que el bloque existe y está ocupado
            if(ext_bytemaps->bmap_bloques[inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]] == 1 )
               printf(" %d", inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]);
         }
         printf("\n");
      }
   }
}

//Esta función imprime los bytemaps
void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
        int i;
        printf("Inodos:");
        //i empieza en la posición 0 y llega hasta MAX_INODOS -1, va imprimiendo el
        //número/carácter en cada posición con espacios.
        for(i=0; i < MAX_INODOS; i++){
                printf(" %u", ext_bytemaps->bmap_inodos[i]);
        }
        //Lo mismo que arriba pero con el bytemap de bloques.
        printf("\nBloques [0-25] :");
        for(i=0; i < 25; i++){
                printf(" %u", ext_bytemaps->bmap_bloques[i]);
        }
        printf("\n");
}

//Rename de ficheros en un directorio dado un nombre existente de fichero y uno nuevo
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BYTE_MAPS *ext_bytemaps, char *comando){
	char dummy[LONGITUD_COMANDO];
	char nombrenuevo[LONGITUD_COMANDO];
	char nombreantiguo[LONGITUD_COMANDO];
   //Fragmentamos el comando en tres partes
	sscanf(comando, "%s %s %s", dummy, nombreantiguo, nombrenuevo);

   //Comprobacion de nombre nuevo
	for(int i=1; i<MAX_FICHEROS; i++){
                if(strcmp(directorio[i].dir_nfich, nombrenuevo) == 0){
			if(ext_bytemaps->bmap_inodos[directorio[i].dir_inodo] == 1){
                                printf("ERROR: nombre de fichero en uso [%s]", nombrenuevo);
				return 0;
                        }
		}
	}

   //Comprobacion de nombre de fichero a renombrar
	for(int i=1; i<MAX_FICHEROS; i++){
		if(strcmp(directorio[i].dir_nfich, nombreantiguo) == 0){
			if(ext_bytemaps->bmap_inodos[directorio[i].dir_inodo] == 1){
				strcpy(directorio[i].dir_nfich, nombrenuevo);
					return 1;
			}
		}
	}
   printf("ERROR: el fichero no existe [%s]", nombreantiguo);
	return 0;
}


int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *comando){

        char dummy[LONGITUD_COMANDO];
	char nombre[LONGITUD_COMANDO];
        //Fragmentamos el comando en dos partes
	sscanf(comando, "%s %s", dummy, nombre);

        //Comprobacion de nombre existente
	for(int i=1; i<MAX_FICHEROS; i++){
                if(strcmp(directorio[i].dir_nfich, nombre) == 0){
			if(ext_bytemaps->bmap_inodos[directorio[i].dir_inodo] == 1){
                                //Marcar 7 punteros a bloque del inodo con 0xFFFF
				ext_superblock->s_free_inodes_count++;
                                for(int j=0; j<MAX_NUMS_BLOQUE_INODO; j++){
                                        //Comprobamos que el bloque existe y está ocupado
                                        if(ext_bytemaps->bmap_bloques[inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]] == 1 ){
                                                //Ponemos a 0 el bloque del bytemap
						ext_superblock->s_free_blocks_count++;
                                                ext_bytemaps->bmap_bloques[inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]] = 0;
                                                //Marcamos el puntero correspondiente del inodo con FFFFH
                                                inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j] = 0xFFFF;
                                        }
                                }
                                //Marcamos el inodo con 0
                                ext_bytemaps->bmap_inodos[directorio[i].dir_inodo] = 0;

                                //Poner tamaño 0 en el inodo liberado
                                inodos->blq_inodos[directorio[i].dir_inodo].size_fichero = 0;

                                //Borrar entradas del directorio
                                directorio[i].dir_nfich[LEN_NFICH] = *("");
                                directorio[i].dir_inodo = 0xFFFF;

				return 1;
                        }
		}
	}
        printf("ERROR: fichero %s no encontrado\n", nombre);
        return 0;
}

int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, FILE *fich, char *comando){

	char dummy[LONGITUD_COMANDO];
        char nombrenuevo[LONGITUD_COMANDO];
        char nombreantiguo[LONGITUD_COMANDO];
	int pos_directorio=0;
	int inodos_fich[MAX_INODOS];
   //Fragmentamos el comando en tres partes
        sscanf(comando, "%s %s %s", dummy, nombreantiguo, nombrenuevo);

	//Comprobador de máximo de ficheros
	int ficheros_actuales=0;
        for(int i=1; i<MAX_FICHEROS; i++){
                if(ext_bytemaps->bmap_inodos[directorio[i].dir_inodo] == 1)
                        ficheros_actuales++;
        }
        if(ficheros_actuales==MAX_FICHEROS){
                printf("ERROR: maximo de ficheros alcanzado [%d]", ficheros_actuales);
		return 0;
	}

	//Comprobrar que el nombre nuevo no exista y que el nombre antiguo si);
	for(int i=1; i<MAX_FICHEROS; i++){
                if(strcmp(directorio[i].dir_nfich, nombrenuevo) == 0){
                        if(ext_bytemaps->bmap_inodos[directorio[i].dir_inodo] == 1){
                                printf("ERROR: nombre de fichero en uso [%s]", nombrenuevo);
                                return 0;
                        }
                }
		if(strcmp(directorio[i].dir_nfich, nombreantiguo) == 0){
                        if(ext_bytemaps->bmap_inodos[directorio[i].dir_inodo] == 1){
                                pos_directorio = i;
                        }
                }
        }
	if(pos_directorio==0){
		printf("ERROR: el nombre del fichero a copiar no existe [%s]", nombreantiguo);
		return 0;
	}

	//Sacamos los bloques que ocupa el fichero a copiar
	int cantidad_bloques=0;
	for(int j=0; j<MAX_NUMS_BLOQUE_INODO; j++){
		//Comprobamos que el bloque existe y está ocupado
		if(ext_bytemaps->bmap_bloques[inodos->blq_inodos[directorio[pos_directorio].dir_inodo].i_nbloque[j]] == 1 ){
			inodos_fich[cantidad_bloques]=inodos->blq_inodos[directorio[pos_directorio].dir_inodo].i_nbloque[j];
			cantidad_bloques++;
		}
	}
	//Sacamos los bloques que va a ocupar el fichero nuevo
	if(cantidad_bloques > ext_superblock->s_free_blocks_count){
		printf("ERROR: el fichero a copiar es mayor que el tamanyo disponible [%d > %d]", cantidad_bloques, ext_superblock->s_free_blocks_count);
		return 0;
	} else {
		ext_superblock->s_free_blocks_count = ext_superblock->s_free_blocks_count - cantidad_bloques;
		ext_superblock->s_free_inodes_count--;
	}

	//montaje del nuevo fichero
	ficheros_actuales++;
	//Ocupación del inodo
	for(int i=1; i<MAX_INODOS; i++){
		if(ext_bytemaps->bmap_inodos[i]==0){
			directorio[ficheros_actuales].dir_inodo = i;
			strcpy(directorio[ficheros_actuales].dir_nfich, nombrenuevo);
			ext_bytemaps->bmap_inodos[i]=1;
			break;
		}
	}
	//Ocupacion de los bloques
	for(int j=0; j<cantidad_bloques;j++){
		for(int i=1; i<MAX_BLOQUES_PARTICION;i++){
			if(ext_bytemaps->bmap_bloques[i]==0){
				inodos->blq_inodos[directorio[ficheros_actuales].dir_inodo].i_nbloque[j] = i;
				for(int o=0; o<SIZE_BLOQUE;o++){
					memdatos[i-4].dato[o] = memdatos[inodos_fich[j]-4].dato[o];
				}
				ext_bytemaps->bmap_bloques[i]=1;
				break;
			}
		}
	}
	inodos->blq_inodos[directorio[ficheros_actuales].dir_inodo].size_fichero = inodos->blq_inodos[directorio[pos_directorio].dir_inodo].size_fichero;
	printf("Archivo copiado con exito!");
	return 1;
}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *comando, EXT_BYTE_MAPS *bytemaps){
        char dummy[LONGITUD_COMANDO];
	char nombre[LONGITUD_COMANDO];
        //Fragmentamos el comando en dos partes
	sscanf(comando, "%s %s", dummy, nombre);

        int existe = 0;
        //Comprobacion de nombre existente
        for(int i=1; i<MAX_FICHEROS; i++){
                if(strcmp(directorio[i].dir_nfich, nombre) == 0){
                        if(bytemaps->bmap_inodos[directorio[i].dir_inodo] == 1){
                                //Se marca la posición del inodo del fichero que existe
			        existe = i;
                        }
		}
	}
        //Si el bucle acaba y no hay matches, se sale de la función
        if(existe == 0){
                printf("ERROR: Fichero %s no encontrado.\n", nombre);
                return 0;
        }
        //si se encuentra, se imprime los bloques en secuencia
        for(int j = 0; j < MAX_NUMS_BLOQUE_INODO; j++){
                //se comprueba que el bloque existe para evitar problemas con datos basura
                if(bytemaps->bmap_bloques[inodos->blq_inodos[directorio[existe].dir_inodo].i_nbloque[j]] == 1){
			for(int i=0; i<SIZE_BLOQUE;i++){
				printf("%c", memdatos[inodos->blq_inodos[directorio[existe].dir_inodo].i_nbloque[j]-4].dato[i]);
			}
                }
        }
}
