#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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

int entrada(Ring *ring, int node) {     // entra um nó no anel
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
	updateFingerTable(ring);

	return 1;
}

int saida (Ring *ring, int node) {      // exclui um nó do anel
	int i, j;

	// Verifica se o nó existe no anel
	for (i = 0; i < ring->size; i++) {
		if (ring->nodes[i].N == node) {
			break;
    	}
		if (i==ring->size-1) {
			printf("Error: Nó não existe no anel.\n");
			return -1;
		}
	}

	// Desloca os nós para a esquerda
	for (j=i; j<ring->size; j++) {
		ring->nodes[j] = ring->nodes[j+1];
	}
	ring->size--;

	// transferKeys(Ring *ring, node);
	updateFingerTable(ring);

	return 1;
}

int lookup (Ring *ring, int node, int key, int timestamp) {     // procura uma chave no anel
	return 1;
}

int inclusao (Ring *ring, int key) {   // inclui uma chave no anel
	int i;

	for (i=0; i<ring->size; i++) {
		if (key < ring->nodes[i].N) {
			ring->nodes[i].HashTable[ring->nodes[i].HT_size].key = key;
			ring->nodes[i].HashTable[ring->nodes[i].HT_size].value = key % MAX_SIZE;
			ring->nodes[i].HT_size++;
			break;
		}
		else if (i == ring->size) {	// chave é maior que o último valor no anel, inserimos no primeiro
			ring->nodes[0].HashTable[ring->nodes[0].HT_size].key = key;
			ring->nodes[0].HashTable[ring->nodes[0].HT_size].value = key % MAX_SIZE;
			ring->nodes[0].HT_size++;
		}
	}
	return 1;
}

void updateFingerTable(Ring *ring) {
	int i, j, m;
	for (i=0; i<ring->size; i++) {
		int key;
		ring->nodes[i].FT_size = (int)ceil(log2(ring->nodes[ring->size-1].N + 1));

		for (j=0; j<ring->nodes[i].FT_size; j++) {
			key = ring->nodes[i].N + ring->nodes[i].FingerTable[j].index;	// ex: N8 + 16
			for (m=0; m<ring->size; m++) {	// procura nó correspondente
				if (ring->nodes[m].N > key) {
					ring->nodes[i].FingerTable[j].node = ring->nodes[m].N;
					break;
				}
				if (m==ring->size-1)	// não há nenhum nó maior, atribui ao primeiro do anel
					ring->nodes[i].FingerTable[j].node = ring->nodes[0].N;
			}
		}
	}
}