#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "hash.h"

// Inicializa a finger table com tamanho MAX_SIZE e define todas as posições como vazias
FingerTable *inicializaFingerTable(void) {
	FingerTable* FT = (FingerTable*) malloc(MAX_SIZE * sizeof(FingerTable));
	if (FT == NULL) {
    	printf("Error: memory allocation.\n");
    	exit(1);
	}
	for (int i = 0; i < MAX_SIZE; i++) {
    	FT[i].index = pow(2, i);
		FT[i].node = 0;
	}
	return FT;
}

// Inicializa a hash table com tamanho HASH_SIZE e define todas as posições como vazias
HashTable *inicializaHashTable(void) {
  	HashTable* HT = (HashTable*) malloc(MAX_SIZE * sizeof(HashTable));
	if (HT == NULL) {
    	printf("Error: memory allocation.\n");
		exit(1);
	}
	for (int i = 0; i < MAX_SIZE; i++) {
    	HT[i].key = 0;
		HT[i].value = 0;
	}
	return HT;
}

// Inicializa um vetor de nós com tamanho MAX_SIZE e define todas as posições como vazias
Node *inicializaNodes(void) {
	Node* nodes = (Node*) malloc(MAX_SIZE * sizeof(Node));
	if (nodes == NULL) {
    	printf("Error: memory allocation.\n");
    	exit(1);
	}
	for (int i = 0; i < MAX_SIZE; i++) {
		nodes[i].FingerTable = inicializaFingerTable();
		nodes[i].FT_size = 0;
		nodes[i].HashTable = inicializaHashTable();
		nodes[i].HT_size = 0;
		nodes[i].N = 0;
	}
	return nodes;
}

// Inicializa o anel com tamanho MAX_SIZE
Ring *inicializaRing(void) {
	Ring *ring = (Ring*) malloc(sizeof(Ring));
	if (ring == NULL) {
		printf("Error: memory allocation.\n");
		exit(1);
	}
	ring->nodes = inicializaNodes();
	ring->size = 0;

	return ring;
}

// Entra um nó no anel
int entrada(Ring *ring, int node) {
	int i, j;

	// Verifica se o nó já existe no anel
	for (i = 0; i < ring->size; i++) {
		if (ring->nodes[i].N == node) {
			printf("Error: Nó já existe no anel.\n");
			return -1;
    	}
	}

	// Encontra a posição correta para inserir o nó de forma ordenada
	for (i = 0; i < ring->size; i++) {
		if (ring->nodes[i].N > node) {
			break;
		}
	}

	// Desloca os nós para a direita para abrir espaço para o novo nó
	for (j = ring->size; j > i; j--) {
		ring->nodes[j] = ring->nodes[j - 1];
	}
	ring->size++;

	// Insere o novo nó
	ring->nodes[i].N = node;
	ring->nodes[i].FT_size = 0;
	ring->nodes[i].HT_size = 0;
	updateKeys(ring, node);
	updateFingerTable(ring);

	return 1;
}

// Exclui um nó do anel
int saida (Ring *ring, int node) {
	int i, j;

	transferKeys(ring, node);

	i = findNode(ring, node);
	// Desloca os nós para a esquerda
	for (j=i; j<ring->size; j++) {
		ring->nodes[j] = ring->nodes[j+1];
	}
	ring->size--;

	updateFingerTable(ring);

	return 1;
}

// Procura uma chave no anel
int lookup (Ring *ring, int node, int key, int timestamp, int *lookup_nodes, int *lookup_count) {
	int i, j, closest, closest_node, diff;

	lookup_nodes[*lookup_count] = node;
	(*lookup_count)++;
	i = findNode(ring, node);

	// testa se o nó armazena a chave requisitada
	for (j=0; j<ring->nodes[i].HT_size; j++)
		if (ring->nodes[i].HashTable[j].key == key)
			return ring->nodes[i].N;

	closest = MAX_SIZE;
	// encontra nó mais próximo ao valor da chave
	for (j=0; j<ring->nodes[i].FT_size; j++) {
		diff = abs(ring->nodes[i].FingerTable[j].node - key);
		if (diff < closest) {
			closest = diff;
			closest_node = ring->nodes[i].FingerTable[j].node;
		}
	}
	
	return lookup(ring, closest_node, key, timestamp, lookup_nodes, lookup_count);
}

// Inclui uma chave no anel
int inclusao (Ring *ring, int node, int key) {
	int i;

	i = findNode(ring, node);
	
	// Verificação de limite da hash table
    if (ring->nodes[i].HT_size >= MAX_SIZE) {
        printf("Error: Hash table full for node %d\n", node);
        return -1;
    }
	
	for (; i<ring->size; i++) {
		if (key <= ring->nodes[i].N) {
			ring->nodes[i].HashTable[ring->nodes[i].HT_size].key = key;
			ring->nodes[i].HashTable[ring->nodes[i].HT_size].value = key % MAX_SIZE;
			ring->nodes[i].HT_size++;
			break;
		}
		else if (i == ring->size-1) {	// chave é maior que o último valor no anel, inserimos no primeiro
			ring->nodes[0].HashTable[ring->nodes[0].HT_size].key = key;
			ring->nodes[0].HashTable[ring->nodes[0].HT_size].value = key % MAX_SIZE;
			ring->nodes[0].HT_size++;
			break;
		}
	}

	return 1;
}

void updateFingerTable(Ring *ring) {
	int i, j, m;
	for (i=0; i<ring->size; i++) {
		int key;
		ring->nodes[i].FT_size = (int)ceil(log2(ring->nodes[ring->size-1].N + 1));	// tamanho da finger table

		for (j=0; j<ring->nodes[i].FT_size; j++) {
			ring->nodes[i].FingerTable[j].node = 0;
			key = (ring->nodes[i].N + ring->nodes[i].FingerTable[j].index) % (int)pow(2, ring->nodes[i].FT_size);	// (N+2^(k-1))mod 2^m
			for (m=i+1; m!=i;m = (m + 1) % ring->size) {	// procura nó correspondente
				if (ring->nodes[m].N >= key) {
					ring->nodes[i].FingerTable[j].node = ring->nodes[m].N;
					break;
				}
			}
		}
	}
}

// transfere as chaves de um nó para o seu sucessor
void transferKeys(Ring *ring, int node) {
	int new_node, sucessor, j, shift;

	// i = findNode(ring, node);

	new_node = findNode(ring, node);
	sucessor = new_node+1;
	if (new_node==ring->size-1)
		sucessor = 0;

	shift = ring->nodes[new_node].HT_size;
	// Desloca as chaves para baixo para abrir espaço para as novas chaves
	for (j = ring->nodes[sucessor].HT_size; j > 0; j--) {
		ring->nodes[sucessor].HashTable[j+shift-1].key = ring->nodes[sucessor].HashTable[j - 1].key;
		ring->nodes[sucessor].HashTable[j+shift-1].value = ring->nodes[sucessor].HashTable[j - 1].value;
	}
	// copia as novas chaves vindas do nó deletado
	for (j = 0; j<shift; j++) {
		ring->nodes[sucessor].HashTable[j].key = ring->nodes[new_node].HashTable[j].key;
		ring->nodes[sucessor].HashTable[j].value = ring->nodes[new_node].HashTable[j].value;

		// limpa hash table do nó deletado
		ring->nodes[new_node].HashTable[j].key = 0;
		ring->nodes[new_node].HashTable[j].value = 0;
		ring->nodes[new_node].HT_size = 0;
	}
	ring->nodes[sucessor].HT_size+=shift;
}

// um novo nó entrou no anel, roubar os possíveis valores da Hash Table de seus vizinhos
int updateKeys(Ring *ring, int node) {
	int i, j, new_node, sucessor, anterior;
	new_node = findNode(ring, node);

	sucessor = new_node+1;
	anterior = new_node-1;
	if (new_node==0) {
		anterior = ring->size-1;
	}
	if (new_node==ring->size-1) {
		sucessor = 0;
	}

	if (ring->size > 1) {
		// copia chaves do sucessor
		for (i=0; i<ring->nodes[sucessor].HT_size; i++) {
			if (node >= ring->nodes[sucessor].HashTable[i].key || (node > ring->nodes[sucessor].N && ring->nodes[sucessor].N < ring->nodes[sucessor].HashTable[i].key)) {
				// copia chaves
				ring->nodes[new_node].HashTable[ring->nodes[new_node].HT_size].key = ring->nodes[sucessor].HashTable[i].key;
				ring->nodes[new_node].HashTable[ring->nodes[new_node].HT_size].value = ring->nodes[sucessor].HashTable[i].value;
				ring->nodes[new_node].HT_size++;

				// Desloca as chaves para cima
				for (j=i; j<ring->nodes[sucessor].HT_size; j++) {
					ring->nodes[sucessor].HashTable[j].key = ring->nodes[sucessor].HashTable[j+1].key;
					ring->nodes[sucessor].HashTable[j].value = ring->nodes[sucessor].HashTable[j+1].value;
				}
				ring->nodes[sucessor].HT_size--;
			}
		}

		// copia chaves do anterior
		for (i=0; i<ring->nodes[anterior].HT_size; i++) {
			if (ring->nodes[anterior].N < ring->nodes[anterior].HashTable[i].key) {
				// copia chaves
				ring->nodes[new_node].HashTable[ring->nodes[new_node].HT_size].key = ring->nodes[anterior].HashTable[i].key;
				ring->nodes[new_node].HashTable[ring->nodes[new_node].HT_size].value = ring->nodes[anterior].HashTable[i].value;
				ring->nodes[new_node].HT_size++;

				// Desloca as chaves para cima
				for (j=i; j<ring->nodes[anterior].HT_size; j++) {
					ring->nodes[anterior].HashTable[j].key = ring->nodes[anterior].HashTable[j+1].key;
					ring->nodes[anterior].HashTable[j].value = ring->nodes[anterior].HashTable[j+1].value;
				}
				ring->nodes[anterior].HT_size--;
			}
		}
	}
	return 1;
}

// retorna índice do nó no anel
int findNode (Ring *ring, int node) {
	int i;
	for (i=0; i<ring->size; i++) {
		if (ring->nodes[i].N == node)
			break;
		if (i==ring->size-1) {
			printf("Error: Nó %d não existe no anel.\n", node);
			return -1;
		}
	}
	return i;
}

void printHashTable(Ring* ring, int node) {
	int i = findNode(ring, node);
	printf("\nHash Table do %d\n", node);
	for (int j=0; j<ring->nodes[i].HT_size; j++) {
		printf("    |%d || %d|\n", ring->nodes[i].HashTable[j].key, ring->nodes[i].HashTable[j].value);
	}
	printf("\n");
}

void printFingerTable(Ring* ring, int node) {
	int i = findNode(ring, node);
	printf("\nFinger Table do %d\n", node);
	for (int j=0; j<ring->nodes[i].FT_size; j++) {
		printf("    |%d || %d|\n", ring->nodes[i].FingerTable[j].index, ring->nodes[i].FingerTable[j].node);
	}
	printf("\n");
}