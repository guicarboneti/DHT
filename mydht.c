#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "utils.h"

int main (void) {
    Ring *ring = inicializaRing();

    executa_entrada(ring);
    free(ring);
    
    return 0;
}