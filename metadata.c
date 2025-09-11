#include <stdio.h>
#include <string.h>
#include <dirent.h>  // Para leer directorios



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metadata.h"

void metadata_inicializar(Metadata *meta) {
    if (meta == NULL) return;
    
    strcpy(meta->archivo, "");
    meta->Largo_PK = 0;
    meta->Pos_PK = -1;
    meta->LRECL = 0;
    meta->contiene_FK = 0;
    strcpy(meta->FK, "");
    meta->Pos_FK = -1;
}
char* metadata_nombre(Metadata* meta){
    return meta->archivo;
}

void metadata_mostrar(const Metadata *meta) {
    if (meta == NULL) {
        printf("Metadata: NULL\n");
        return;
    }
    
    printf("=== METADATA ===\n");
    printf("Archivo: %s\n", meta->archivo);
    printf("Largo PK: %d\n", meta->Largo_PK);
    printf("Posición PK: %d\n", meta->Pos_PK);
    printf("Longitud Registro (LRECL): %d\n", meta->LRECL);
    printf("Contiene FK: %d\n", meta->contiene_FK);
    if(meta->contiene_FK == 1){
        printf("Clave Foránea (FK): %s\n", meta->FK);
        printf("Posición FK: %d\n", meta->Pos_FK);
    }
    printf("================\n");
}

int metadata_guardar(const Metadata *meta, const char *filename) {
    if (meta == NULL || filename == NULL) {
        printf("Error: Parámetros inválidos\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error al abrir archivo para escritura");
        return 0;
    }

    fprintf(file, "ARCHIVO=%s\n", meta->archivo);
    fprintf(file, "LARGO_PK=%d\n", meta->Largo_PK);
    fprintf(file, "POS_PK=%d\n", meta->Pos_PK);
    fprintf(file, "LRECL=%d\n", meta->LRECL);
    fprintf(file, "CONTIENE_FK=%d\n", meta->contiene_FK);
    fprintf(file, "FK=%s\n", meta->FK);
    fprintf(file, "POS_FK=%d\n", meta->Pos_FK);

    fclose(file);
    return 1;
}

int metadata_cargar(Metadata *meta, const char *filename) {
    if (meta == NULL || filename == NULL) {
        printf("Error: Parámetros inválidos\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir archivo para lectura");
        return 0;
    }

    char line[MAX_STR * 2];
    char key[MAX_STR];
    char value[MAX_STR];

    metadata_inicializar(meta);

    while (fgets(line, sizeof(line), file)) {
        // Eliminar salto de línea
        line[strcspn(line, "\n")] = '\0';
        
        if (sscanf(line, "%[^=]=%[^\n]", key, value) == 2) {
            if (strcmp(key, "ARCHIVO") == 0) {
                strncpy(meta->archivo, value, MAX_STR - 1);
                meta->archivo[MAX_STR - 1] = '\0';
            } else if (strcmp(key, "LARGO_PK") == 0) {
                meta->Largo_PK = atoi(value);
            } else if (strcmp(key, "POS_PK") == 0) {
                meta->Pos_PK = atoi(value);
            } else if (strcmp(key, "LRECL") == 0) {
                meta->LRECL = atoi(value);
            } else if (strcmp(key, "CONTIENE_FK") == 0) {
                meta->contiene_FK = atoi(value);
            } else if (strcmp(key, "FK") == 0) {
                strncpy(meta->FK, value, MAX_STR - 1);
                meta->FK[MAX_STR - 1] = '\0';
            } else if (strcmp(key, "POS_FK") == 0) {
                meta->Pos_FK = atoi(value);
            }
        }
    }

    fclose(file);
    return 1;
}

int metadata_existe(const char *filename) {
    if (filename == NULL) {
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

