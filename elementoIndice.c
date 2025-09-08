#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include"elementoIndice.h"



// Crear un nuevo elemento de índice
elementoIndice crear_elemento(int pk, int pos) {
    elementoIndice elem;
    elem.pk = pk;
    elem.pos = pos;
    elem.borrado = 0;
    elem.referencias = 0;
    return elem;
}
