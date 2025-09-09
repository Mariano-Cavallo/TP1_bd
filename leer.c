#include <stdio.h>
#include <string.h>
#include "TP1_bd/gestorDeIndices.h"

int leer(){
 char archivo[16];
 char indice[16];
 printf("Ingrese nombre del archivo: ");
 fgets(archivo, sizeof(archivo), stdin);
 archivo[strcspn(archivo, "\n")] = '\0';

 strcpy(indice, archivo);
 strcat(indice, ".idx");
 strcat(archivo, ".txt");


 char pk[16];
 printf("ingrese la pk: ");
 fgets(pk,sizeof(pk),stdin);
 pk[strcspn(archivo, "\n")] = '\0';

 FILE *f =fopen(indice,"r");
 //int posicion = buscar_elemento_indice();
 int posicion = 2; // esto es de prueba suponiendo que la posicion es la 2
 fclose(f);

 FILE *f_data =fopen(archivo,"r");
 char buffer[256];
 int count = 0;
 while (fgets(buffer, sizeof(buffer), f_data)) {
        count++;

        if (count == posicion) {
            printf("Linea %d: %s", count, buffer);
            break;
        }
    }

}
