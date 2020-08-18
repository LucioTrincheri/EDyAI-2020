#include <string.h>
#include <stdlib.h>
#include "hash.h"

Hash *hash_crear(unsigned int capacidad) {
  Hash *tabla = malloc(sizeof(Hash));
  tabla->largo = capacidad;
  tabla->lista = calloc(capacidad, sizeof(HashLista));
  return tabla;
}

// Hasheo djb2
unsigned int hash_indice(int largo, char *alias) {
  unsigned int hash = 5381;
  int c;
  while ((c = *alias++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash % largo;
}

// Funciones DLIST adaptadas a una lista de DLISTS (copia y mejora de dlist.c)

void hash_insertar(Hash *tabla, char *alias, AVLTree arbol) {
  unsigned int indice;
  // Se obtienen la key del alias.
  indice = hash_indice(tabla->largo, alias);

  // Recorro la lista enlazada para ver si ya esta el alias
  for (HashLista *aux = tabla->lista[indice]; aux != NULL; aux = aux->siguiente) {
    if (strcmp(alias, aux->alias) == 0){
      itree_destruir(aux->conjunto);
      aux->conjunto = arbol;
      return;
    }
  }
  // Inicializo el nodo
  HashLista *nodo = malloc(sizeof(HashLista));
  // Guardo la información
  nodo->alias = alias;
  nodo->conjunto = arbol;
  // Agrego el nodo al inicio
  nodo->siguiente = tabla->lista[indice];
  // Apunto el inicio de la lista al nuevo nodo
  tabla->lista[indice] = nodo;
}

AVLTree hash_conjunto(Hash *tabla, char *alias) {
  unsigned int indice;
  indice = hash_indice(tabla->largo, alias);
  HashLista *nodo;

  // Busca en la lista.
  for (nodo = tabla->lista[indice]; nodo != NULL && strcmp(alias, nodo->alias) != 0; nodo = nodo->siguiente);
  return nodo ? nodo->conjunto : NULL;
}

void hash_destruir(Hash *tabla) {
  HashLista* nodo;
  HashLista* proximo;
  for (unsigned int indice = 0; indice < tabla->largo; ++indice) {
    proximo = tabla->lista[indice];
    while (proximo){
      nodo = proximo;
      proximo = nodo->siguiente;
      free(nodo->alias);
      itree_destruir(nodo->conjunto);
      free(nodo);
    }
  }
  free(tabla->lista);
  free(tabla);
}
