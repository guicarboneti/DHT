#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "utils.h"

int main (void) {
    Ring *ring = inicializaRing();
    // Saida* saidas = (Saida*) malloc(M * sizeof(Saida));

    // lê e executa entrada
    executa_entrada(ring);
    
    // Prepara a saída com os dados ordenados
    // saida(saidas, T1, T2, &n);
    // ordena_saida(saidas, n);
    // imprime_saida(saidas, n);
    
    free(ring);
    // free(T2);   
    // free(saidas);
    
    return 0;
}