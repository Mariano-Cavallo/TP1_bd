#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "TP1.h"

typedef struct {
    char comando[20];
    void (*funcion)();
    char descripcion[50];
} Comando;


// Lista de comandos disponibles
Comando comandos[] = {
    {"ayuda", ayuda, "muestra los comandos"},
    {"crear", crear, "crea un archivo con pk"},
    {"insertar", insertar, "inserta en un archivo una pk"},
    {"borrar", borrar, "borra una pk de un archivo"},
    {"leer", leer, "muestra el contenido de un registro de una pk"},
    {"mostrar", mostrar, "muestra el contenido de un archivo"},
    {"cambiar", cambiar, "cambia el contenido de una pk en un archivo"},
    {"salir", salir, "Termina el programa"}

};

int num_comandos = sizeof(comandos) / sizeof(comandos[0]);

// Función para limpiar la entrada
void limpiar_entrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Función para convertir a minúsculas
void a_minusculas(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void ejecutar_comando(char *entrada) {
    char comando[50];
    sscanf(entrada, "%s", comando);
    a_minusculas(comando);
    
    for (int i = 0; i < num_comandos; i++) {
        if (strcmp(comando, comandos[i].comando) == 0) {
            comandos[i].funcion();
            return;
        }
    }
    
    printf("Error: Comando '%s' no reconocido. Escribe 'ayuda' para ver los comandos disponibles.\n", comando);
}


void iniciar(){
    int cantidadArchivosAlInicio;
    Metadata meta1;
    gestorIndices *gestorIndices = crear_gestor_indices();
    GestorDeMetadata *gestor = gestor_crear("./");
    cantidadArchivosAlInicio = gestor_cantidad(gestor);
  

    gestor_cargar_directorio(gestor);
    gestor_listar(gestor);
    metadata_mostrar(gestor_buscar(gestor,"clientes"));
    metadata_mostrar(gestor_buscar(gestor,"sucursales"));


    return;

}

void ayuda() {
    printf("\n=== COMANDOS DISPONIBLES ===\n");
    for (int i = 0; i < num_comandos; i++) {
        printf("%-10s - %s\n", comandos[i].comando, comandos[i].descripcion);
    }
    printf("============================\n\n");
}

void crear(){
    return;
}
void insertar(){
    return;
}
void borrar(){
    return;
}
void leer(){
    return;
}
void mostrar(){
    return;
}
void cambiar(){
    return;
}
void salir(){
    return;
}