#ifndef ELEMENTO_INDICE_H  
#define ELEMENTO_INDICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct elementoIndice {
	int pk;
	int pos;
	int borrado;
    int referencias;
} elementoIndice;

elementoIndice crear_elemento(int, int);

#endif