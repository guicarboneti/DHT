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
            int *lookup_nodes = (int *) calloc(MAX_SIZE, sizeof(int));
        	printf("out: %d\n", node);
            printFingerTable(ring, node);
            printHashTable(ring, node);
            int final = lookup(ring, node, key, timestamp, lookup_nodes);
        	printf("final: %d\n", final);
            printf("%d L %d {", timestamp, key);
            for (i=0; ; i++) {
                if (lookup_nodes[i]==0)
                    break;
                printf("%d,", lookup_nodes[i]);
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