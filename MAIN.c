#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#include"gestorDeIndices.h"
#include"indice.h"
#include"elementoIndice.h"



int main() {
    
    printf("=== SISTEMA DE GESTIÓN DE MÚLTIPLES ÍNDICES ===\n\n");
    
    // 1. Crear el gestor de índices
    gestorIndices* gestor = crear_gestor_indices();
    if (gestor == NULL) {
        printf("Error: No se pudo crear el gestor de índices\n");
        return 1;
    }
    
    // 2. Crear tres índices diferentes
    printf("Creando índices...\n");
    crear_nuevo_indice(gestor, "clientes");
    crear_nuevo_indice(gestor, "productos");
    crear_nuevo_indice(gestor, "empleados");
    
    // 3. Agregar 5 elementos al índice "clientes"
    printf("\n--- Agregando clientes ---\n");
    agregar_elemento_indice(gestor, "clientes", 1001);
    agregar_elemento_indice(gestor, "clientes", 1002);
    agregar_elemento_indice(gestor, "clientes", 1003);
    agregar_elemento_indice(gestor, "clientes", 1004);
    agregar_elemento_indice(gestor, "clientes", 1005);
    printf("5 clientes agregados\n");
    
    // 4. Agregar 5 elementos al índice "productos"
    printf("\n--- Agregando productos ---\n");
    agregar_elemento_indice(gestor, "productos", 2001);
    agregar_elemento_indice(gestor, "productos", 2002);
    agregar_elemento_indice(gestor, "productos", 2003);
    agregar_elemento_indice(gestor, "productos", 2004);
    agregar_elemento_indice(gestor, "productos", 2005);
    printf("5 productos agregados\n");
    
    // 5. Agregar 5 elementos al índice "empleados"
    printf("\n--- Agregando empleados ---\n");
    agregar_elemento_indice(gestor, "empleados", 3001);
    agregar_elemento_indice(gestor, "empleados", 3002);
    agregar_elemento_indice(gestor, "empleados", 3003);
    agregar_elemento_indice(gestor, "empleados", 3004);
    agregar_elemento_indice(gestor, "empleados", 3005);
    printf("5 empleados agregados\n");
    
    // 6. Mostrar todos los índices disponibles
    printf("\n--- Índices creados ---\n");
    //listar_indices(gestor);
    
    // 7. Mostrar el contenido detallado de cada índice
    printf("\n--- Contenido del índice 'clientes' ---\n");
    indice* idx_clientes = obtener_indice_gestor(gestor, "clientes");
    if (idx_clientes != NULL) {
        mostrar_indice(idx_clientes);
    }
    
    printf("\n--- Contenido del índice 'productos' ---\n");
    indice* idx_productos = obtener_indice_gestor(gestor, "productos");
    if (idx_productos != NULL) {
        mostrar_indice(idx_productos);
    }
    
    printf("\n--- Contenido del índice 'empleados' ---\n");
    indice* idx_empleados = obtener_indice_gestor(gestor, "empleados");
    if (idx_empleados != NULL) {
        mostrar_indice(idx_empleados);
    }
    
    // 8. Realizar algunas búsquedas de ejemplo
    printf("\n--- Realizando búsquedas ---\n");
    
    // Buscar cliente 1003
    int pos = buscar_elemento_indice(gestor, "clientes", 1003);
    if (pos != -1) {
        printf("Cliente 1003 encontrado - Posición en archivo: %d\n", pos);
    }
    
    // Buscar producto 2005
    pos = buscar_elemento_indice(gestor, "productos", 2005);
    if (pos != -1) {
        printf("Producto 2005 encontrado - Posición en archivo: %d\n",pos);
    }
    
    // Buscar empleado 3002
    pos = buscar_elemento_indice(gestor, "empleados", 3002);
    if (pos != -1) {
        printf("Empleado 3002 encontrado - Posición en archivo: %d\n",pos);
    }
    
    // 10. Mostrar estadísticas finales
    printf("\n--- Estadísticas finales ---\n");
    printf("Total de índices: %d\n", gestor->cantidad);
    
    int total_elementos = 0;
    for (int i = 0; i < gestor->cantidad; i++) {
        total_elementos += gestor->indices[i]->cantidad;
        printf("Índice '%s': %d elementos\n", 
               gestor->nombres[i], gestor->indices[i]->cantidad);
    }
    printf("Total de elementos en todos los índices: %d\n", total_elementos);
    
    // 11. Liberar recursos
    printf("\n--- Liberando recursos ---\n");
    liberar_gestor_indices(gestor);
    printf("Gestor de índices liberado. Programa terminado.\n");

    
    return 0;
}