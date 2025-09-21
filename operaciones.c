#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metadata.h"
#include "gestorDeIndices.h"

// ==================================================
// INSERTAR
// ==================================================
int insertar(Metadata *meta, const char *registro) {
    if ((int)strlen(registro) != meta->LRECL) {
        printf("Error: longitud inválida del registro\n");
        return -1;
    }

    // Extraer PK
    char pk[128];
    strncpy(pk, registro + meta->Pos_PK, meta->Largo_PK);
    pk[meta->Largo_PK] = '\0';

    // Verificar duplicado
    if (existePK(meta->archivo, pk)) { // TODO: conectar a tu índice real
        printf("Error: PK duplicada\n");
        return -1;
    }

    // Validar FK
    if (strcmp(meta->FK, "NONE") != 0) {
        char fk[128];
        strncpy(fk, registro + meta->Pos_FK, meta->Largo_PK);
        fk[meta->Largo_PK] = '\0';

        if (!existePK(meta->FK, fk)) { // TODO
            printf("Error: FK inexistente\n");
            return -1;
        }
    }

    // Archivo de datos
    char datafile[256];
    strcpy(datafile, meta->archivo);
    datafile[strlen(datafile) - 5] = '\0'; // quitar ".meta"
    strcat(datafile, ".dat");

    FILE *f = fopen(datafile, "a");
    if (!f) {
        printf("Error: no se pudo abrir archivo de datos\n");
        return -1;
    }
    fprintf(f, "%s\n", registro);
    fclose(f);

    // Actualizar índice
    // TODO: usar gestorDeIndices
    // insertar_en_indice(meta->archivo, pk, posicion);

    printf("Registro insertado correctamente.\n");
    return 0;
}

// ==================================================
// BORRAR
// ==================================================
int borrar(Metadata *meta, const char *pk) {
    int pos = buscarPK(meta->archivo, pk); // TODO
    if (pos < 0) {
        printf("Error: PK no existe\n");
        return -1;
    }

    // Verificar referencias de FK
    if (tieneReferenciasFK(meta, pk)) { // TODO
        printf("Error: PK tiene referencias activas\n");
        return -1;
    }

    char datafile[256];
    strcpy(datafile, meta->archivo);
    datafile[strlen(datafile) - 5] = '\0';
    strcat(datafile, ".dat");

    FILE *f = fopen(datafile, "r+");
    if (!f) {
        printf("Error al abrir archivo de datos.\n");
        return -1;
    }

    // Mover al registro y marcar borrado
    fseek(f, pos, SEEK_SET);
    fputc('b', f); // Marca lógica de borrado
    fclose(f);

    borrarDeIndice(meta->archivo, pk); // TODO

    printf("Registro borrado correctamente.\n");
    return 0;
}

// ==================================================
// CAMBIAR
// ==================================================
int cambiar(Metadata *meta, const char *pk_ant, const char *nuevo_reg) {
    if (buscarPK(meta->archivo, pk_ant) < 0) {
        printf("Error: PK anterior no existe\n");
        return -1;
    }

    if (borrar(meta, pk_ant) < 0) {
        return -1;
    }

    if (insertar(meta, nuevo_reg) < 0) {
        return -1;
    }

    printf("Registro cambiado correctamente.\n");
    return 0;
}
