#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "utils.h"

// le arquivo de entrada e executa as operações
// void executa_entrada(Ring *ring) {
//     char op, line[MAX_SIZE];
//     int timestamp, node, key;

//     while (fgets(line, sizeof(line), stdin)) {
//         sscanf(line, "%d %c %d %d", &timestamp, &op, &node, &key);

//         if (op == 'E') {
//             printf("entra nó %d\n", node);
//             entrada(ring, node);
//         } 
//         else if (op == 'S') {
//             printf("sai nó %d\n", node);
//             saida(ring, node);
//         } 
//         else if (op == 'L') {
//             printf("%d lookup %d\n", node, key);
//             // for (int i=0; i<ring->size; i++) {
//             //     printFingerTable(ring, ring->nodes[i].N);
//             //     printHashTable(ring, ring->nodes[i].N);
//             // }
//             // int *lookup_nodes = (int *) calloc(MAX_SIZE, sizeof(int));
//             // int lookup_count = 0;
//             // lookup(ring, node, key, timestamp, lookup_nodes, &lookup_count);
//             // imprime_saida(ring, timestamp, key, lookup_nodes, lookup_count);
//             // free(lookup_nodes);
//         } 
//         else if (op == 'I') {
//             printf("%d inclui %d\n", node, key);
//             inclusao(ring, node, key);
//         }
//     }
// }

void executa_entrada(Ring *ring) {
    char op;
    int timestamp, node, key;

    while (scanf("%d %c %d %d", &timestamp, &op, &node, &key) != EOF)
        if (op == 'E') {
            // printf("entra nó %d\n", node);
            // for (int i=0; i<ring->size; i++) {
            //     printFingerTable(ring, ring->nodes[i].N);
            //     printHashTable(ring, ring->nodes[i].N);
            // }
            entrada(ring, node);
        }
        else if (op == 'S') {
            // printf("sai nó %d\n", node);
            saida(ring, node);
            // for (int i=0; i<ring->size; i++) {
            //     printFingerTable(ring, ring->nodes[i].N);
            //     printHashTable(ring, ring->nodes[i].N);
            // }
        }
        else if (op == 'L') {
            printf("%d lookup %d\n", node, key);
            for (int i=0; i<ring->size; i++) {
                printFingerTable(ring, ring->nodes[i].N);
                printHashTable(ring, ring->nodes[i].N);
            }
            int *lookup_nodes = (int *) calloc(MAX_SIZE, sizeof(int));
            int lookup_count=0;
            lookup(ring, node, key, timestamp, lookup_nodes, &lookup_count);
            imprime_saida(ring, timestamp, key, lookup_nodes, lookup_count);
        }
        else if (op == 'I') {
            // printf("%d inclui %d\n", node, key);
            inclusao(ring, node, key);
            // for (int i=0; i<ring->size; i++) {
            //     printFingerTable(ring, ring->nodes[i].N);
            //     printHashTable(ring, ring->nodes[i].N);
            // }
        }
        else
            printf("ERRO: Operação desconhecida: %c\n", op);
}

void imprime_saida(Ring *ring, int timestamp, int key, int *lookup_nodes, int lookup_count) {
    int i, j, FT_size;
	printf("%d L %d {", timestamp, key);
	for (i=0; i<lookup_count; i++) {
		if (i==0)
			printf("%d", lookup_nodes[i]);
		else
			printf(",%d", lookup_nodes[i]);
	}
	printf("}\n");
    for (i=0; i<lookup_count; i++) {
        FingerTable* FT = calculaFingerTable(ring, lookup_nodes[i], &FT_size);
        printf("%d T %d {", timestamp, lookup_nodes[i]);
        for (j=0; j<FT_size; j++)
            if (j==0)
                printf("%d", FT[j].node);
            else
                printf(",%d", FT[j].node);
        printf("}\n");
        free(FT);
    }

}