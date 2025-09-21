#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "gestorMeta.h"


int gestor_cantidad(GestorDeMetadata *gestor){
    return gestor->cantidad;
}

// Función para crear un nuevo gestor (asigna memoria)
GestorDeMetadata* gestor_crear(const char *directorio) {
    GestorDeMetadata *gestor = (GestorDeMetadata*)malloc(sizeof(GestorDeMetadata));
    if (gestor == NULL) {
        perror("Error al asignar memoria para el gestor");
        return NULL;
    }
    
    gestor_inicializar(gestor, directorio);
    return gestor;
}

// Función para liberar la memoria del gestor
void gestor_destruir(GestorDeMetadata *gestor) {
    if (gestor != NULL) {
        free(gestor);
    }
}

Metadata* metadata_buscar_por_id(GestorDeMetadata*gestor,int id){
    return &gestor->metadatas[id-1];
}

void gestor_inicializar(GestorDeMetadata *gestor, const char *directorio) {
    gestor->cantidad = 0;
    if (directorio != NULL) {
        strncpy(gestor->directorio, directorio, MAX_STR - 1);
        gestor->directorio[MAX_STR - 1] = '\0';
        
        // Crear el directorio si no existe
        #ifdef _WIN32
            mkdir(directorio);
        #else
            mkdir(directorio, 0755);
        #endif
    } else {
        strcpy(gestor->directorio, "./");
    }

    
    // Inicializar todas las metadatas
    for (int i = 0; i < MAX_METADATAS; i++) {
        metadata_inicializar(&gestor->metadatas[i]);
    }
    

}

int gestor_agregar(GestorDeMetadata *gestor, const Metadata *meta) {
    if (gestor->cantidad >= MAX_METADATAS) {
        printf("Error: No se pueden agregar más metadatas (límite: %d)\n", MAX_METADATAS);
        return 0;
    }
    
    // Verificar si ya existe
    for (int i = 0; i < gestor->cantidad; i++) {
        if (strcmp(gestor->metadatas[i].archivo, meta->archivo) == 0) {
            printf("Error: Ya existe metadata para el archivo '%s'\n", meta->archivo);
            return 0;
        }
    }
    
    // Agregar la nueva metadata
    gestor->metadatas[gestor->cantidad] = *meta;
    gestor->cantidad++;
    
    return 1;
}

Metadata* gestor_buscar(GestorDeMetadata *gestor, const char *nombre_archivo) {
    for (int i = 0; i < gestor->cantidad; i++) {
        if (strcmp(gestor->metadatas[i].archivo, nombre_archivo) == 0) {
            return &gestor->metadatas[i];
        }
    }
    return NULL;
}

void gestor_listar(const GestorDeMetadata *gestor) {
    printf("\n=== LISTADO DE METADATAS (%d) ===\n", gestor->cantidad);
    printf("Directorio: %s\n", gestor->directorio);
    printf("==================================\n");
    
    if (gestor->cantidad == 0) {
        printf("No hay metadatas cargadas.\n");
        return;
    }
    
    for (int i = 0; i < gestor->cantidad; i++) {
        printf("%d. %s \n", 
               i + 1, 
               gestor->metadatas[i].archivo);
    }
    printf("==================================\n");
}

int gestor_cargar_directorio(GestorDeMetadata *gestor) {
    DIR *dir;
    struct dirent *entry;
    char filepath[MAX_STR * 2];
    int cargadas = 0;
    
    dir = opendir(gestor->directorio);
    if (dir == NULL) {
        perror("Error al abrir directorio");
        return 0;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        // Buscar archivos .meta
        if (strstr(entry->d_name, ".meta") != NULL) {
            snprintf(filepath, sizeof(filepath), "%s/%s", gestor->directorio, entry->d_name);
            
            Metadata meta;
            if (metadata_cargar(&meta, filepath)) {
                if (gestor_agregar(gestor, &meta)) {
                    cargadas++;
                }
            }
        }
    }
    
    closedir(dir);
    return cargadas;
}


/*
int gestor_guardar_todo(GestorDeMetadata *gestor) {
    int guardadas = 0;
    char filepath[MAX_STR * 2];
    
    for (int i = 0; i < gestor->cantidad; i++) {
        snprintf(filepath, sizeof(filepath), "%s/%s.meta", 
                gestor->directorio, gestor->metadatas[i].archivo);
        
        if (guardar_metadata(&gestor->metadatas[i], filepath)) {
            guardadas++;
        }
    }
    
    return guardadas;
}

int gestor_guardar_metadata(GestorDeMetadata *gestor, const char *nombre_archivo) {
    Metadata *meta = gestor_buscar(gestor, nombre_archivo);
    if (meta == NULL) {
        printf("Error: No se encontró metadata para '%s'\n", nombre_archivo);
        return 0;
    }
    
    char filepath[MAX_STR * 2];
    snprintf(filepath, sizeof(filepath), "%s/%s.meta", 
            gestor->directorio, nombre_archivo);
    
    return guardar_metadata(meta, filepath);
}

int gestor_cargar_metadata(GestorDeMetadata *gestor, const char *nombre_archivo) {
    char filepath[MAX_STR * 2];
    snprintf(filepath, sizeof(filepath), "%s/%s.meta", 
            gestor->directorio, nombre_archivo);
    
    Metadata meta;
    if (cargar_metadata(&meta, filepath)) {
        // Eliminar si ya existe
        gestor_eliminar(gestor, nombre_archivo);
        return gestor_agregar(gestor, &meta);
    }
    
    return 0;
}
*/