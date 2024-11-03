#ifndef _HASH_H
#define _HASH_H
#define MAX_SIZE 50

typedef struct FingerTable {
    int index;
    int node;
} FingerTable;

typedef struct HashTable {
    int key;
    int value;
} HashTable;

typedef struct Node {
    int N;
    FingerTable *FingerTable;
    HashTable *HashTable;
    int FT_size;
    int HT_size;
} Node;

typedef struct Ring {
    int size;
    Node *nodes;
} Ring;

FingerTable *inicializaFingerTable(void);
HashTable *inicializaHashTable(void);
Node *inicializaNodes(void);
Ring *inicializaRing(void);

int entrada(Ring *ring, int node);     // entra um nó no anel

int saida (Ring *ring, int node);      // exclui um nó do anel

int lookup (Ring *ring, int node, int key, int timestamp);     // procura uma chave no anel

int inclusao (Ring *ring, int key);   // inclui uma chave no anel

void updateFingerTable(Ring *ring);

#endif