#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include"gestorDeIndices.h"

#define CAPACIDAD_INICIAL 15


// Crear gestor
gestorIndices* crear_gestor_indices() {
    gestorIndices* gestor = (gestorIndices*)malloc(sizeof(gestorIndices));
    if (gestor == NULL) return NULL;
    
    gestor->indices = (indice**)malloc(CAPACIDAD_INICIAL * sizeof(indice*));
    gestor->nombres = (char**)malloc(CAPACIDAD_INICIAL * sizeof(char*));
    
    if (gestor->indices == NULL || gestor->nombres == NULL) {
        free(gestor->indices);
        free(gestor->nombres);
        free(gestor);
        return NULL;
    }
    
    gestor->cantidad = 0;
    gestor->capacidad = CAPACIDAD_INICIAL;
    return gestor;
}

// Liberar gestor
void liberar_gestor_indices(gestorIndices* gestor) {
    if (gestor == NULL) return;
    
    for (int i = 0; i < gestor->cantidad; i++) {
        liberar_indice(gestor->indices[i]);
        free(gestor->nombres[i]);
    }
    
    free(gestor->indices);
    free(gestor->nombres);
    free(gestor);
}

// Agregar índice existente
//agregar warning si se crean mas de 15 indices
int agregar_indice_gestor(gestorIndices* gestor, const char* nombre, indice* idx) {
    if (gestor == NULL || nombre == NULL || idx == NULL) return -1;
    
    // Verificar si ya existe
    for (int i = 0; i < gestor->cantidad; i++) {
        if (strcmp(gestor->nombres[i], nombre) == 0) {
            return 0; // Ya existe
        }
    }
    
    // Agregar nuevo índice
    gestor->nombres[gestor->cantidad] = strdup(nombre);
    if (gestor->nombres[gestor->cantidad] == NULL) {
        return -1;
    }
    
    gestor->indices[gestor->cantidad] = idx;
    gestor->cantidad++;
    
    return 1;
}

// Crear nuevo índice
int crear_nuevo_indice(gestorIndices* gestor, const char* nombre) {
    indice *nuevo = crear_indice();
    if (nuevo == NULL) return -1;
    
    int resultado = agregar_indice_gestor(gestor, nombre, nuevo);
    if (resultado <= 0) {
        liberar_indice(nuevo);
    }
    
    return resultado;
}

// Obtener índice por nombre
indice* obtener_indice_gestor(const gestorIndices* gestor, const char* nombre) {
    if (gestor == NULL || nombre == NULL) return NULL;
    
    for (int i = 0; i < gestor->cantidad; i++) {
        if (strcmp(gestor->nombres[i], nombre) == 0) {
            return gestor->indices[i];
        }
    }
    
    return NULL;
}

// Listar índices
void listar_indices(const gestorIndices* gestor) {
    if (gestor == NULL || gestor->cantidad == 0) {
        printf("No hay índices disponibles\n");
        return;
    }
    
    printf("Índices disponibles (%d):\n", gestor->cantidad);
    for (int i = 0; i < gestor->cantidad; i++) {
        printf("%d. %s (%d elementos)\n", i + 1, gestor->nombres[i], 
               gestor->indices[i]->cantidad);
    }
}

// Operaciones específicas en índices
int agregar_elemento_indice(gestorIndices* gestor, const char* nombre, int pk) {
    indice* idx = obtener_indice_gestor(gestor, nombre);
    if (idx == NULL) return -1;
    
    return agregar_elemento(idx, pk);
}

int buscar_elemento_indice(const gestorIndices* gestor, const char* nombre, int pk) {
    indice* idx = obtener_indice_gestor(gestor, nombre);
    if (idx == NULL) return -1;
    
    return busqueda_binaria_por_pk(idx, pk);
}

// Eliminar un índice del gestor por nombre
int eliminar_indice_gestor(gestorIndices* gestor, const char* nombre) {
    if (gestor == NULL || nombre == NULL) {
        return -1;
    }
    
    // Buscar el índice
    int posicion = -1;
    for (int i = 0; i < gestor->cantidad; i++) {
        if (strcmp(gestor->nombres[i], nombre) == 0) {
            posicion = i;
            break;
        }
    }
    
    if (posicion == -1) {
        return 0; // Índice no encontrado
    }
    
    // Liberar memoria del índice
    liberar_indice(gestor->indices[posicion]);
    free(gestor->nombres[posicion]);
    
    // Mover los elementos restantes
    for (int i = posicion; i < gestor->cantidad - 1; i++) {
        gestor->indices[i] = gestor->indices[i + 1];
        gestor->nombres[i] = gestor->nombres[i + 1];
    }
    
    gestor->cantidad--;
    
    return 1; 
}

// Eliminar elemento de un índice específico por PK
int eliminar_elemento_indice(gestorIndices* gestor, const char* nombre, int pk) {
    if (gestor == NULL || nombre == NULL) {
        return -1;
    }
    
    // Obtener el índice
    indice* idx = obtener_indice_gestor(gestor, nombre);
    if (idx == NULL) {
        return 0; // Índice no encontrado
    }
    
    // Buscar el elemento usando búsqueda binaria (asumiendo que está ordenado por PK)
    int posicion = busqueda_binaria_por_pk(idx, pk);
    if (posicion == -1) {
        return 0; // Elemento no encontrado
    }
    
    // Realizar borrado lógico
    idx->elemento[posicion].borrado = 1;
    
    return 1; // Éxito
}

// Guardar todos los índices
// int guardar_todos_indices(const gestorIndices* gestor, const char* directorio) {
//     if (gestor == NULL || directorio == NULL) return -1;
    
//     for (int i = 0; i < gestor->cantidad; i++) {
//         char filename[256];
//         snprintf(filename, sizeof(filename), "%s/%s.dat", directorio, gestor->nombres[i]);
        
//         if (guardar_indice(filename, gestor->indices[i]) != 0) {
//             return -1;
//         }
//     }
    
//     return 0;
// }
