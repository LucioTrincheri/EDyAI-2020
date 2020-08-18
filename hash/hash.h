#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED
#include "../avltree/avltree.h"
// El hash es una 

// Nodo de la lista enlazada
typedef struct _HashLista {
    AVLTree conjunto;
    char *alias;
    struct _HashLista *siguiente;
} HashLista;

// Puntero tabla hash
typedef struct {
    // Lista enlazada
    HashLista **lista;
    unsigned int largo;
} Hash;

Hash *hash_crear(unsigned int);

unsigned int hash_indice(int, char *);

void hash_insertar(Hash *, char *, AVLTree);

AVLTree hash_conjunto(Hash *, char *);

void hash_destruir(Hash *);

#endif