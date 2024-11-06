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
        if (op == 'E') {
            entrada(ring, node);
        }
        else if (op == 'S') {
            saida(ring, node);
        }
        else if (op == 'L') {
            // for (int i =0; i<ring->size; i++) {
            //     printHashTable(ring, ring->nodes[i].N);
            //     printFingerTable(ring, ring->nodes[i].N);
            // }
            int *lookup_nodes = (int *) calloc(MAX_SIZE, sizeof(int));
            int lookup_count=0;
            lookup(ring, node, key, timestamp, lookup_nodes, &lookup_count);
            imprime_saida(ring, timestamp, key, lookup_nodes, lookup_count);
        }
        else if (op == 'I') {
            inclusao(ring, node, key);
        }
        else
            printf("ERRO: Operação desconhecida: %c\n", op);
}

void imprime_saida(Ring *ring, int timestamp, int key, int *lookup_nodes, int lookup_count) {
    int i, j, node_idx;
	printf("%d L %d {", timestamp, key);
	for (i=0; i<lookup_count; i++) {
		if (i==0)
			printf("%d", lookup_nodes[i]);
		else
			printf(",%d", lookup_nodes[i]);
	}
	printf("}\n");
    for (i=0; i<lookup_count; i++) {
        node_idx = findNode(ring, lookup_nodes[i]);
        printf("%d T %d {", timestamp, lookup_nodes[i]);
        for (j=0; j<ring->nodes[node_idx].FT_size; j++)
            if (j==0)
                printf("%d", ring->nodes[node_idx].FingerTable[j].node);
            else
                printf(",%d", ring->nodes[node_idx].FingerTable[j].node);
        printf("}\n");
    }

}