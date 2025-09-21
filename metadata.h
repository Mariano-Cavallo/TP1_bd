#ifndef METADATA_H
#define METADATA_H

#define MAX_FILES 100
#define MAX_STR 256

typedef struct {
    char archivo[MAX_STR];  
    int Largo_PK;
    int Pos_PK;
    int LRECL;
    int contiene_FK;
    char FK[MAX_STR];
    int Pos_FK;
} Metadata;



// Prototipos de funciones
char* metadata_nombre(Metadata*);

void metadata_inicializar(Metadata *);

void metadata_mostrar(const Metadata *);

int metadata_guardar(const Metadata *, const char *);

int metadata_cargar(Metadata *, const char *);

int metadata_existe(const char *);




#endif