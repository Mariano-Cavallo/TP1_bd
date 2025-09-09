#include <stdio.h>
#include <string.h>
#include <dirent.h>  // Para leer directorios

#define MAX_FILES 100
#define MAX_STR 256

typedef struct {
    char archivo[MAX_STR];  // Nuevo campo: nombre del archivo
    char PK[MAX_STR];
    int Largo_PK;
    int Pos_PK;
    int LRECL;
    char FK[MAX_STR];
    int Pos_FK;
} Metadata;

int main() {
    Metadata metadatas[MAX_FILES];
    int count = 0;

    DIR *directorio = opendir(".");  // Abrir directorio actual
    if (directorio == NULL) {
        printf("No se pudo abrir el directorio.\n");
        return 1;
    }

    struct dirent *entrada;
    while ((entrada = readdir(directorio)) != NULL) {
        // Revisar si el archivo termina en ".meta"
        if (strstr(entrada->d_name, ".meta") != NULL) {
            FILE *archivo = fopen(entrada->d_name, "r");
            if (archivo == NULL) {
                printf("No se pudo abrir el archivo %s\n", entrada->d_name);
                continue;
            }

            Metadata m;
            strcpy(m.archivo, entrada->d_name);  // Guardar nombre del archivo
            char linea[MAX_STR];

            while (fgets(linea, sizeof(linea), archivo)) {
                sscanf(linea, "PK: %s", m.PK);
                sscanf(linea, "Largo_PK: %d", &m.Largo_PK);
                sscanf(linea, "Pos_PK: %d", &m.Pos_PK);
                sscanf(linea, "LRECL: %d", &m.LRECL);
                sscanf(linea, "FK: %s", m.FK);
                sscanf(linea, "Pos_FK: %d", &m.Pos_FK);
            }

            fclose(archivo);
            metadatas[count++] = m;
            if (count >= MAX_FILES) break; // Evitar pasar del tamaño del array
        }
    }

    closedir(directorio);

    // Mostrar datos
    for (int i = 0; i < count; i++) {
        printf("Archivo analizado: %s\n", metadatas[i].archivo);
        printf("PK: %s\nLargo_PK: %d\nPos_PK: %d\nLRECL: %d\nFK: %s\nPos_FK: %d\n\n",
               metadatas[i].PK, metadatas[i].Largo_PK, metadatas[i].Pos_PK,
               metadatas[i].LRECL, metadatas[i].FK, metadatas[i].Pos_FK);
    }

    return 0;
}
