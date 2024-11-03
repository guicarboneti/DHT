#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "utils.h"

// le arquivo de entrada e executa as operações
void executa_entrada(Ring *ring) {
    char op;
    int timestamp, node, key;

    while (scanf("%d %c %d %d", &timestamp, &op, &node, &key) != EOF)
        if (op == 'E')
            entrada(ring, node);
        else if (op == 'S')
            saida(ring, node);
        else if (op == 'L')
            lookup(ring, node, key, timestamp);
        else if (op == 'I')
            inclusao(ring, node, key);
        else
            printf("ERRO: Operação desconhecida: %c\n", op);
}