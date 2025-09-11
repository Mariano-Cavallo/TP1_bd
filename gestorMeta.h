#ifndef GESTOR_METADATA_H
#define GESTOR_METADATA_H

#include "metadata.h"

#define MAX_METADATAS 10

typedef struct {
    Metadata metadatas[MAX_METADATAS];
    int cantidad;
    char directorio[MAX_STR];
} GestorDeMetadata;

// Prototipos de funciones
GestorDeMetadata* gestor_crear(const char *);
Metadata* metadata_buscar_por_id(GestorDeMetadata*,int);
int gestor_cantidad(GestorDeMetadata *);
void gestor_destruir(GestorDeMetadata *);
void gestor_inicializar(GestorDeMetadata *, const char *);
int gestor_agregar(GestorDeMetadata *, const Metadata *);
int gestor_eliminar(GestorDeMetadata *, const char *);
Metadata* gestor_buscar(GestorDeMetadata *, const char *);
void gestor_listar(const GestorDeMetadata *);
int gestor_cargar_directorio(GestorDeMetadata *);


#endif // GESTOR_METADATA_H