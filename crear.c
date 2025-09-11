#include <stdio.h>
#include <string.h>

int main() {
    char nombre[16];
    printf("Ingrese nombre del archivo: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    char pk[4];
    printf("Ingrese nombre de la pk: ");
    fgets(pk, sizeof(pk), stdin);
    pk[strcspn(pk, "\n")] = '\0';

    char buffer[16] = "";

    int largo_pk;
    printf("Ingrese largo de la pk: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &largo_pk);

    int pos_pk;
    printf("Ingrese posicion de la pk: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &pos_pk);

    int lrecl;
    printf("Ingrese largo del registro: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &lrecl);

    int pos_fk;
    printf("Ingrese posicion de la fk (presione ENTER para omitir): ");
    fgets(buffer, sizeof(buffer), stdin);

    int fk_omitida = 0;
    char archivo_fk[16];
    if (buffer[0] == '\n') {
        printf("FK omitida\n");
        fk_omitida = 1;
    } else {
        sscanf(buffer, "%d", &pos_fk);
        printf("valor %d\n", pos_fk);

        printf("Ingrese nombre del archivo de la fk: ");
        fgets(archivo_fk, sizeof(archivo_fk), stdin);
        archivo_fk[strcspn(archivo_fk, "\n")] = '\0';
        // validar fk: traewr pk del archivo mencionado.
        // abrir archivo_fk.meta
        // estraer el campo n'1 del archivo con sscanf
        char fk_name[16];
        FILE *f_meta =fopen(archivo_fk,"r");
        char linea[250];
        fgets(linea, sizeof(linea), f_meta);
        sscanf(linea, "PK: %s", fk_name);
        printf("nombre fk: %s", fk_name);


    }

    // Crear archivo
    char nombre_archivo[16];
    snprintf(nombre_archivo, sizeof(nombre_archivo), "%s.txt", nombre);
    FILE *f_output = fopen(nombre_archivo, "w");
    if (f_output == NULL) {
        perror("Error al abrir archivo principal");
        return 1;
    }
    fclose(f_output);

    // Crear archivo de indices
    char nombre_indices[16];
    snprintf(nombre_indices, sizeof(nombre_indices), "%s.idx", nombre);
    FILE *f_index = fopen(nombre_indices, "w");
    if (f_index == NULL) {
        perror("Error al abrir archivo de indices");
        return 1;
    }
    fclose(f_index);

    // Crear archivo de metadata
    char nombre_metadata[16];
    snprintf(nombre_metadata, sizeof(nombre_metadata), "%s.meta", nombre);
    FILE *f_meta = fopen(nombre_metadata, "w");
    if (f_meta == NULL) {
        perror("Error al abrir archivo de metadata");
        return 1;
    }
    fprintf(f_meta, "PK: %s\nLargo_PK: %d\nPos_PK: %d\nLRECL: %d\n", pk, largo_pk, pos_pk, lrecl);
    if (!fk_omitida) {
        fprintf(f_meta, "FK: %s\nPos_FK: %d\n", archivo_fk, pos_fk);
    }
    fclose(f_meta);

    printf("Archivos creados correctamente.\n");
    return 0;
}
