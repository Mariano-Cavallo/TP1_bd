#ifndef INDICE_H  
#define INDICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"elementoIndice.h"

typedef struct indice {
	elementoIndice* elemento;
	int cantidad;
    int siguientePos;
}indice;


indice* crear_indice();

void liberar_indice(indice*);

void mostrar_indice(const indice* );

int agregar_elemento(indice*, int);

int buscar_por_pk(const indice*, int);

int borrar_elemento_logico(indice* , int);

int incrementar_referencias(indice*, int);

int obtener_posicion(const indice*, int);

int comparar_por_posicion(const void*, const void*);

void ordenar_por_posicion(indice*);

int busqueda_binaria_por_pk(const indice*, int);

int esta_ordenado_por_pk(const indice*);

#endif