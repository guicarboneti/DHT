#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "utils.h"

int main (void) {
    Ring *ring = inicializaRing();
    // Saida* saidas = (Saida*) malloc(M * sizeof(Saida));

    // lê e executa entrada
    executa_entrada(ring);
    
    printf("\n-----\n");
    for (int i=0; i<ring->size; i++) {
        printf("\n-%d-\nFinger Table\n", ring->nodes[i].N);
        for (int j=0; j<ring->nodes[i].FT_size; j++) {
            printf("    |%d || %d|\n", ring->nodes[i].FingerTable[j].index, ring->nodes[i].FingerTable[j].node);
        }
        printf("\nHash Table\n");
        printf("%d\n", ring->nodes[i].HT_size);
        for (int j=0; j<ring->nodes[i].HT_size; j++) {
            printf("    |%d || %d|\n", ring->nodes[i].HashTable[j].key, ring->nodes[i].HashTable[j].value);
        }
    }
    printf("\n");

    // Prepara a saída com os dados ordenados
    // saida(saidas, T1, T2, &n);
    // ordena_saida(saidas, n);
    // imprime_saida(saidas, n);
    
    free(ring);
    // free(T2);   
    // free(saidas);
    
    return 0;
}