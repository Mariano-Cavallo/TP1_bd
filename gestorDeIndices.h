#ifndef GESTOR_H  
#define GESTOR_H


#include"indice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Estructura del gestor
typedef struct gestorIndices {
    indice** indices;
    char** nombres;
    int cantidad;
    int capacidad;
} gestorIndices;


gestorIndices* crear_gestor_indices();

void liberar_gestor_indices(gestorIndices*);

int agregar_indice_gestor(gestorIndices*, const char* , indice* );

int crear_nuevo_indice(gestorIndices* , const char*);

int eliminar_indice_gestor(gestorIndices* , const char*);

indice* obtener_indice_gestor(const gestorIndices* , const char*);

int agregar_elemento_indice(gestorIndices* , const char* , int);

int buscar_elemento_indice(const gestorIndices* , const char* , int);

int eliminar_elemento_indice(gestorIndices* , const char* , int);

#endif