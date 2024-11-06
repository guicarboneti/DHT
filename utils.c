#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "utils.h"

// le arquivo de entrada e executa as operações
void executa_entrada(Ring *ring) {
    char op;
    int timestamp, node, key, i;

    while (scanf("%d %c %d %d", &timestamp, &op, &node, &key) != EOF)
        if (op == 'E') {
            printf("entra %d\n", node);
            entrada(ring, node);
        }
        else if (op == 'S') {
            printf("sai %d\n", node);
            saida(ring, node);
        }
        else if (op == 'L') {
            printf("%d lookup %d\n", node, key);
            for (int i =0; i<ring->size; i++) {
                printHashTable(ring, ring->nodes[i].N);
                printFingerTable(ring, ring->nodes[i].N);
            }
            int *lookup_nodes = (int *) calloc(MAX_SIZE, sizeof(int));
            int lookup_count=0;
            lookup(ring, node, key, timestamp, lookup_nodes, &lookup_count);
            printf("%d L %d {", timestamp, key);
            for (i=0; i<lookup_count; i++) {
                if (i==0)
                    printf("%d", lookup_nodes[i]);
                else
                    printf(",%d", lookup_nodes[i]);
            }
            printf("}\n");
        }
        else if (op == 'I') {
            printf("%d inclui %d\n", node, key);
            inclusao(ring, node, key);
        }
        else
            printf("ERRO: Operação desconhecida: %c\n", op);
}