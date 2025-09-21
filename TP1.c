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

// Variables globales para los gestores (puedes moverlas a otro lugar si prefieres)
static gestorIndices *g_indices = NULL;
static GestorDeMetadata *g_meta = NULL;

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


void iniciar() {
    // Crear gestores globales
    if (!g_indices) g_indices = crear_gestor_indices();
    if (!g_meta) g_meta = gestor_crear("./");

    // Cargar todas las metadatas del directorio
    gestor_cargar_directorio(g_meta);

    // Cargar todos los índices del directorio
    DIR *dir = opendir("./");
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            size_t len = strlen(entry->d_name);
            if (len > 4 && strcmp(entry->d_name + len - 4, ".idx") == 0) {
                // Obtener el nombre base del archivo (sin extensión)
                char nombre[100];
                strncpy(nombre, entry->d_name, len - 4);
                nombre[len - 4] = '\0';

                // Crear el índice en el gestor y cargarlo desde archivo
                crear_nuevo_indice(g_indices, nombre);
                indice *idx = obtener_indice_gestor(g_indices, nombre);
                char path[128];
                snprintf(path, sizeof(path), "./%s", entry->d_name);
                indice_cargar(idx, path);
            }
        }
        closedir(dir);
    }
}

// Inicialización de gestores (llamar en iniciar)
void inicializar_gestores() {
    if (!g_indices) g_indices = crear_gestor_indices();
    if (!g_meta) g_meta = gestor_crear("./");
    gestor_cargar_directorio(g_meta);
}

// AYUDA
void ayuda() {
    printf("\n=== COMANDOS DISPONIBLES ===\n");
    for (int i = 0; i < num_comandos; i++) {
        printf("%-10s - %s\n", comandos[i].comando, comandos[i].descripcion);
    }
    printf("============================\n\n");
}

// CREAR archivo y su metadata
void crear() {
    inicializar_gestores();
    char nombre[50];
    printf("Nombre del archivo a crear: ");
    scanf("%49s", nombre);
    limpiar_entrada();

    if (gestor_buscar(g_meta, nombre)) {
        printf("Ya existe un archivo con ese nombre.\n");
        return;
    }

    Metadata meta;
    metadata_inicializar(&meta);
    strncpy(meta.archivo, nombre, sizeof(meta.archivo)-1);

    printf("Largo de la PK: ");
    scanf("%d", &meta.Largo_PK);
    printf("Posición de la PK: ");
    scanf("%d", &meta.Pos_PK);
    printf("Longitud del registro (LRECL): ");
    scanf("%d", &meta.LRECL);
    printf("¿Contiene FK? (1=Sí, 0=No): ");
    scanf("%d", &meta.contiene_FK);
    limpiar_entrada();

    if (meta.contiene_FK) {
        printf("Nombre de la FK: ");
        scanf("%49s", meta.FK);
        printf("Posición de la FK: ");
        scanf("%d", &meta.Pos_FK);
        limpiar_entrada();
    }

    if (gestor_agregar(g_meta, &meta)) {
        char filename[100];
        snprintf(filename, sizeof(filename), "./%s.meta", nombre);
        metadata_guardar(&meta, filename);
        snprintf(filename, sizeof(filename), "./%s.dat", nombre);
        FILE *f = fopen(filename, "w"); if (f) fclose(f);
        // Crear índice vacío en el gestor de índices
        crear_nuevo_indice(g_indices, nombre);
        printf("Archivo, metadata e índice creados correctamente.\n");
    } else {
        printf("Error al crear metadata.\n");
    }
}

// INSERTAR registro en archivo
void insertar() {
    inicializar_gestores();
    char nombre[50];
    printf("Archivo destino: ");
    scanf("%49s", nombre);
    limpiar_entrada();

    Metadata *meta = gestor_buscar(g_meta, nombre);
    if (!meta) {
        printf("No existe ese archivo.\n");
        return;
    }

    char registro[256];
    printf("Ingrese el registro completo (longitud %d): ", meta->LRECL);
    fgets(registro, sizeof(registro), stdin);
    registro[strcspn(registro, "\n")] = '\0';

    if ((int)strlen(registro) != meta->LRECL) {
        printf("Longitud incorrecta.\n");
        return;
    }

    // Obtener PK numérica
    char pkstr[20];
    strncpy(pkstr, registro + meta->Pos_PK, meta->Largo_PK);
    pkstr[meta->Largo_PK] = '\0';
    int pk = atoi(pkstr);

    // Validar FK si corresponde
    if (meta->contiene_FK) {
        // Buscar archivo origen de la FK
        Metadata *meta_fk = gestor_buscar(g_meta, meta->FK);
        if (!meta_fk) {
            printf("No existe el archivo de origen de la FK: %s\n", meta->FK);
            return;
        }
        // Obtener valor de la FK del registro
        char fkstr[20];
        strncpy(fkstr, registro + meta->Pos_FK, meta_fk->Largo_PK);
        fkstr[meta_fk->Largo_PK] = '\0';
        int fk = atoi(fkstr);

        // Buscar la FK como PK en el archivo origen
        indice *idx_fk = obtener_indice_gestor(g_indices, meta->FK);
        if (!idx_fk || busqueda_binaria_por_pk(idx_fk, fk) == -1) {
            printf("La FK ingresada (%d) no existe como PK en el archivo origen (%s).\n", fk, meta->FK);
            return;
        }
    }

    // Obtener o crear índice en el gestor
    indice *idx = obtener_indice_gestor(g_indices, nombre);
    if (!idx) {
        crear_nuevo_indice(g_indices, nombre);
        idx = obtener_indice_gestor(g_indices, nombre);
    }

    // Verificar si ya existe la PK
    if (busqueda_binaria_por_pk(idx, pk) != -1) {
        printf("Ya existe un registro con esa PK.\n");
        return;
    }

    // Insertar en archivo de datos
    char datafile[100];
    snprintf(datafile, sizeof(datafile), "./%s.dat", nombre);
    FILE *f = fopen(datafile, "a+");
    if (!f) {
        printf("No se pudo abrir el archivo de datos.\n");
        return;
    }
    fseek(f, 0, SEEK_END);
    long pos = ftell(f);
    fprintf(f, "%s\n", registro);
    fclose(f);

    // Insertar en índice y guardar
    agregar_elemento(idx, pk);
    idx->elemento[idx->cantidad-1].pos = (int)pos;
    char idxfile[100];
    snprintf(idxfile, sizeof(idxfile), "./%s.idx", nombre);
    indice_guardar(idx, idxfile);

    printf("Registro insertado correctamente.\n");
}

// BORRAR registro por PK
void borrar() {
    inicializar_gestores();
    char nombre[50];
    printf("Archivo: ");
    scanf("%49s", nombre);
    limpiar_entrada();

    Metadata *meta = gestor_buscar(g_meta, nombre);
    if (!meta) {
        printf("No existe ese archivo.\n");
        return;
    }

    char pkstr[20];
    printf("PK a borrar: ");
    scanf("%19s", pkstr);
    limpiar_entrada();
    int pk = atoi(pkstr);

    indice *idx = obtener_indice_gestor(g_indices, nombre);
    if (!idx) {
        printf("No existe el índice para este archivo.\n");
        return;
    }

    int posidx = busqueda_binaria_por_pk(idx, pk);
    if (posidx == -1) {
        printf("PK no encontrada en el índice.\n");
        return;
    }

    // Verificar si esta PK es referenciada como FK en algún otro archivo
    for (int i = 0; i < g_meta->cantidad; i++) {
        Metadata *meta_otro = &(g_meta->metadatas[i]);
        if (meta_otro->contiene_FK && strcmp(meta_otro->FK, nombre) == 0) {
            // Buscar en todos los registros del archivo meta_otro si existe la FK igual a pk
            char datafile[100];
            snprintf(datafile, sizeof(datafile), "./%s.dat", meta_otro->archivo);
            FILE *f = fopen(datafile, "r");
            if (f) {
                char linea[256];
                while (fgets(linea, sizeof(linea), f)) {
                    if (linea[0] == '*') continue; // Saltar borrados lógicos
                    char fkstr[20];
                    strncpy(fkstr, linea + meta_otro->Pos_FK, meta->Largo_PK);
                    fkstr[meta->Largo_PK] = '\0';
                    int fk = atoi(fkstr);
                    if (fk == pk) {
                        printf("No se puede borrar: la PK está referenciada como FK en el archivo %s.\n", meta_otro->archivo);
                        fclose(f);
                        return;
                    }
                }
                fclose(f);
            }
        }
    }

    // Borrado lógico en archivo de datos
    char datafile[100];
    snprintf(datafile, sizeof(datafile), "./%s.dat", nombre);
    FILE *f = fopen(datafile, "r+");
    if (!f) {
        printf("No se pudo abrir el archivo de datos.\n");
        return;
    }
    fseek(f, idx->elemento[posidx].pos, SEEK_SET);
    for (int i = 0; i < meta->LRECL; i++) fputc('*', f);
    fclose(f);

    // Borrado lógico en índice
    idx->elemento[posidx].borrado = 1;
    char idxfile[100];
    snprintf(idxfile, sizeof(idxfile), "./%s.idx", nombre);
    indice_guardar(idx, idxfile);

    printf("Registro borrado lógicamente.\n");
}

// LEER registro por PK
void leer() {
    inicializar_gestores();
    char nombre[50];
    printf("Archivo: ");
    scanf("%49s", nombre);
    limpiar_entrada();

    Metadata *meta = gestor_buscar(g_meta, nombre);
    if (!meta) {
        printf("No existe ese archivo.\n");
        return;
    }

    char pkstr[20];
    printf("PK a buscar: ");
    scanf("%19s", pkstr);
    limpiar_entrada();
    int pk = atoi(pkstr);

    indice *idx = obtener_indice_gestor(g_indices, nombre);
    if (!idx) {
        printf("No existe el índice para este archivo.\n");
        return;
    }

    int posidx = busqueda_binaria_por_pk(idx, pk);
    if (posidx == -1 || idx->elemento[posidx].borrado) {
        printf("PK no encontrada o borrada.\n");
        return;
    }

    char datafile[100];
    snprintf(datafile, sizeof(datafile), "./%s.dat", nombre);
    FILE *f = fopen(datafile, "r");
    if (!f) {
        printf("No se pudo abrir el archivo de datos.\n");
        return;
    }
    fseek(f, idx->elemento[posidx].pos, SEEK_SET);
    char buffer[256];
    fgets(buffer, sizeof(buffer), f);
    fclose(f);

    printf("Registro: %.*s\n", meta->LRECL, buffer);
}

// MOSTRAR todos los registros de un archivo
void mostrar() {
    inicializar_gestores();
    char nombre[50];
    printf("Archivo: ");
    scanf("%49s", nombre);
    limpiar_entrada();

    Metadata *meta = gestor_buscar(g_meta, nombre);
    if (!meta) {
        printf("No existe ese archivo.\n");
        return;
    }

    indice *idx = obtener_indice_gestor(g_indices, nombre);
    if (!idx) {
        printf("No existe el índice para este archivo.\n");
        return;
    }

    char datafile[100];
    snprintf(datafile, sizeof(datafile), "./%s.dat", nombre);
    FILE *f = fopen(datafile, "r");
    if (!f) {
        printf("No se pudo abrir el archivo de datos.\n");
        return;
    }

    printf("Registros activos en %s:\n", nombre);
    for (int i = 0; i < idx->cantidad; i++) {
        if (!idx->elemento[i].borrado) {
            fseek(f, idx->elemento[i].pos, SEEK_SET);
            char buffer[256];
            fgets(buffer, sizeof(buffer), f);
            printf("%.*s\n", meta->LRECL, buffer);
        }
    }
    fclose(f);
}

// CAMBIAR un registro por PK
void cambiar() {
    inicializar_gestores();
    char nombre[50];
    printf("Archivo: ");
    scanf("%49s", nombre);
    limpiar_entrada();

    Metadata *meta = gestor_buscar(g_meta, nombre);
    if (!meta) {
        printf("No existe ese archivo.\n");
        return;
    }

    char pkstr[20];
    printf("PK a modificar: ");
    scanf("%19s", pkstr);
    limpiar_entrada();
    int pk = atoi(pkstr);

    indice *idx = obtener_indice_gestor(g_indices, nombre);
    if (!idx) {
        printf("No existe el índice para este archivo.\n");
        return;
    }

    int posidx = busqueda_binaria_por_pk(idx, pk);
    if (posidx == -1 || idx->elemento[posidx].borrado) {
        printf("PK no encontrada o borrada.\n");
        return;
    }

    char datafile[100];
    snprintf(datafile, sizeof(datafile), "./%s.dat", nombre);
    FILE *f = fopen(datafile, "r");
    if (!f) {
        printf("No se pudo abrir el archivo de datos.\n");
        return;
    }

    printf("Nuevo registro (longitud %d): ", meta->LRECL);
    char nuevo[256];
    fgets(nuevo, sizeof(nuevo), stdin);
    nuevo[strcspn(nuevo, "\n")] = '\0';
    if ((int)strlen(nuevo) != meta->LRECL) {
        printf("Longitud incorrecta.\n");
        fclose(f);
        return;
    }

    fseek(f, idx->elemento[posidx].pos, SEEK_SET);
    fprintf(f, "%s\n", nuevo);
    fclose(f);

    printf("Registro modificado.\n");
}

// SALIR del programa
void salir() {
    printf("Saliendo del sistema...\n");
    if (g_indices) liberar_gestor_indices(g_indices);
    if (g_meta) gestor_destruir(g_meta);
    exit(0);
}