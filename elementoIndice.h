#ifndef ELEMENTO_INDICE_H  
#define ELEMENTO_INDICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
	int pk;
	int pos;
	int borrado;
    int referencias;
} elementoIndice;

elementoIndice crear_elemento(int, int);
elementoIndice crear_elemento_guardad(int, int, int, int);

#endif