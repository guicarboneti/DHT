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
    // FingerTable *FingerTable;
    HashTable *HashTable;
    // int FT_size;
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

int lookup (Ring *ring, int node, int key, int timestamp, int *lookup_nodes, int *lookup_count);     // procura uma chave no anel

int inclusao (Ring *ring, int node, int key);   // inclui uma chave no anel

void updateFingerTable(Ring *ring);

void transferKeys(Ring *ring, int node);

int findNode (Ring *ring, int node);

void printHashTable(Ring* ring, int node);

void printFingerTable(Ring* ring, int node);

int updateKeys(Ring *ring, int node);

FingerTable* calculaFingerTable(Ring *ring, int node, int *FT_size);

#endif