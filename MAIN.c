#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#include"gestorDeIndices.h"
#include"indice.h"
#include"elementoIndice.h"
#include"TP1.h"



int main() {
    
    iniciar();
    
    char entrada[100];
    
    printf("========= SHELL INTERACTIVA =========\n");
    printf("Escribe 'ayuda' para ver los comandos disponibles\n");
    printf("Escribe 'salir' para terminar\n\n");
    
    while (1) {
        printf(">>> ");
        
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            printf("\n");
            break;
        }
        
        // Eliminar el salto de línea
        entrada[strcspn(entrada, "\n")] = '\0';
        
        // Ignorar líneas vacías
        if (strlen(entrada) == 0) {
            continue;
        }
        
        ejecutar_comando(entrada);
    }

    
    return 0;
}