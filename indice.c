#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include"indice.h"
#include"elementoIndice.h"


// Crear una nueva estructura índice
indice* crear_indice() {
    indice* idx = (indice*)malloc(sizeof(indice));
    if (idx != NULL) {
        idx->elemento = NULL;
        idx->cantidad = 0;
    }
    return idx;
}



// Agregar nuevo elemento al índice
int agregar_elemento(indice* idx, int pk) {
    if (idx == NULL) {
        return -1;
    }
    
    // Verificar si ya existe
    if (busqueda_binaria_por_pk(idx, pk) != -1) {
        return 0; // Ya existe
    }
    
    // Redimensionar arreglo?????
    elementoIndice* temp = (elementoIndice*)realloc(idx->elemento, (idx->cantidad + 1) * sizeof(elementoIndice));
    if (temp == NULL) {
        return -1; // Error de memoria
    }
    
    idx->elemento = temp;
    idx->elemento[idx->cantidad] = crear_elemento(pk,idx->cantidad);
    idx->cantidad++;
    ordenar_por_posicion(idx);
    
    return 1; // Éxito
}

// Liberar memoria del índice
void liberar_indice(indice* idx) {
    if (idx != NULL) {
        if (idx->elemento != NULL) {
            free(idx->elemento);
        }
        free(idx);
    }
}

// Mostrar información del índice completo
void mostrar_indice(const indice* idx) {
    if (idx == NULL || idx->elemento == NULL) {
        printf("Índice vacío o no inicializado\n");
        return;
    }
    
    printf("Índice (%d elementos):\n", idx->cantidad);
    printf("PK\tPos\tBorrado\tRefs\n");
    printf("-----------------------------\n");
    
    for (int i = 0; i < idx->cantidad; i++) {
        if (idx->elemento[i].borrado == 0) { // Solo mostrar activos
            printf("%d\t%d\t%d\t%d\n",
                   idx->elemento[i].pk,
                   idx->elemento[i].pos,
                   idx->elemento[i].borrado,
                   idx->elemento[i].referencias);
        }
    }
}


// Buscar elemento por clave primaria
int buscar_por_pk(const indice* idx, int pk) {
    if (idx == NULL || idx->elemento == NULL) {
        return -1;
    }
    
    for (int i = 0; i < idx->cantidad; i++) {
        if (idx->elemento[i].pk == pk && idx->elemento[i].borrado == 0) {
            return i;
        }
    }
    return -1;
}

// Marcar elemento como borrado (borrado lógico)
int borrar_elemento_logico(indice* idx, int pk) {
    int pos = busqueda_binaria_por_pk(idx, pk);
    if (pos != -1) {
        idx->elemento[pos].borrado = 1;
        return 1;
    }
    return 0;
}

// Incrementar contador de referencias
int incrementar_referencias(indice* idx, int pk) {
    int pos = busqueda_binaria_por_pk(idx, pk);
    if (pos != -1) {
        idx->elemento[pos].referencias++;
        return idx->elemento[pos].referencias;
    }
    return -1;
}

// Obtener posición por clave primaria
int obtener_posicion(const indice* idx, int pk) {
    int pos = busqueda_binaria_por_pk(idx, pk);
    if (pos != -1) {
        return idx->elemento[pos].pos;
    }
    return -1;
}

// Función de comparación para qsort
int comparar_por_posicion(const void* a, const void* b) {
    const elementoIndice* elemA = (const elementoIndice*)a;
    const elementoIndice* elemB = (const elementoIndice*)b;
    
    if (elemA->pos < elemB->pos) return -1;
    if (elemA->pos > elemB->pos) return 1;
    return 0;
}

// Ordenar el índice por posición (solo elementos activos)
void ordenar_por_posicion(indice* idx) {
    if (idx == NULL || idx->elemento == NULL || idx->cantidad <= 1) {
        return;
    }
    
    qsort(idx->elemento, idx->cantidad, sizeof(elementoIndice), comparar_por_posicion);
}

// Verificar si el índice está ordenado por PK
int esta_ordenado_por_pk(const indice* idx) {
    if (idx == NULL || idx->elemento == NULL || idx->cantidad <= 1) {
        return 1; // Considerar vacío o un elemento como ordenado
    }
    
    for (int i = 0; i < idx->cantidad - 1; i++) {
        if (idx->elemento[i].borrado == 0 && idx->elemento[i + 1].borrado == 0) {
            if (idx->elemento[i].pk > idx->elemento[i + 1].pk) {
                return 0; // No ordenado
            }
        }
    }
    
    return 1; // Ordenado
}
// Búsqueda binaria en índice ordenado por PK
// Retorna el índice del elemento en el arreglo, o -1 si no se encuentra
int busqueda_binaria_por_pk(const indice* idx, int pk) {
    if (idx == NULL || idx->elemento == NULL || idx->cantidad == 0) {
        return -1;
    }
    
    int izquierda = 0;
    int derecha = idx->cantidad - 1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        // Si encontramos el elemento y no está borrado
        if (idx->elemento[medio].pk == pk) {
            if (idx->elemento[medio].borrado == 0) {
                return medio; // Elemento encontrado
            } else {
                return -1; // Elemento encontrado pero borrado
            }
        }
        
        // Si el elemento del medio es menor, buscar en la mitad derecha
        if (idx->elemento[medio].pk < pk) {
            izquierda = medio + 1;
        } 
        // Si el elemento del medio es mayor, buscar en la mitad izquierda
        else {
            derecha = medio - 1;
        }
    }
    
    return -1; // Elemento no encontrado


}

int indice_guardar(const indice* idx, const char* filename) {
    if (idx == NULL || filename == NULL) {
        printf("Error: Parámetros inválidos\n");
        return 0;
    }
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error al abrir archivo para escritura");
        return 0;
    }
    
    // Guardar cada elemento en formato simple: pk,pos,borrado,referencias
    for (int i = 0; i < idx->cantidad; i++) {
        fprintf(file, "%d,%d,%d,%d\n",
                idx->elemento[i].pk,
                idx->elemento[i].pos,
                idx->elemento[i].borrado,
                idx->elemento[i].referencias);
    }
    
    fclose(file);
    return 1;
}

int indice_cargar(indice* idx, const char* filename) {
    if (idx == NULL || filename == NULL) {
        printf("Error: Parámetros inválidos\n");
        return 0;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir archivo para lectura");
        return 0;
    }
    
    char line[256];
    idx->cantidad = 0; // Reiniciar el contador
    
    while (fgets(line, sizeof(line), file)) {
        // Eliminar salto de línea
        line[strcspn(line, "\n")] = '\0';
        
        int pk, pos, borrado, referencias;
        // Leer los cuatro valores separados por comas
        if (sscanf(line, "%d,%d,%d,%d", &pk, &pos, &borrado, &referencias) == 4) {
            if (!indice_agregar_elemento(idx, pk, pos, borrado, referencias)) {
                fclose(file);
                return 0;
            }
        } else {
            printf("Error: Formato inválido en línea: %s\n", line);
        }
    }
    
    fclose(file);
    return 1;
}

//solo para cargar
int indice_agregar_elemento(indice* idx,int pk,int pos,int borrado,int referencias){

    elementoIndice* temp = (elementoIndice*)realloc(idx->elemento, (idx->cantidad + 1) * sizeof(elementoIndice));
    if (temp == NULL) {
        return -1; // Error de memoria
    }
    
    idx->elemento = temp;
    idx->elemento[idx->cantidad] = crear_elemento_guardad(pk,pos,borrado,referencias);
    idx->cantidad++;
    return 0;


}

