#ifndef LIST_H_INLCUDED
#define LIST_H_INLCUDED

// Estructura que representa un nodo en la lista enlazada,
// cuyo dato es una personta
typedef struct _DNodo DNodo;

// Tipo de la lista, representa el tipo de puntero.
typedef DNodo* DList;

// Tipo de la funcion destruir, para cada tipo de dato sera distinta.
typedef void (*Visitante) (void *, void*);
//typedef void (*VisitanteImprimirArchivo) (FILE*, void *);

// Tipo de funcion que toman los algoritmos de ordenamiento.
typedef int (*Comparacion) (void*, void*);

// Prototipos de funciones de manejo de la lista.

// dlist_crear devuleve la lista vacia.
DList dlist_crear();

// dlist_crear crea la estructura nodo, con los datos pasados en argumentos.
DNodo* dnodo_crear(void* , DNodo*, DNodo*);

// dnodo_crear agrega un nodo a la lista, con los datos determinados (persona).
DList dnodo_agregar_inicio(DList, void*);

// dnodo_agregar imprime los datos guardados en la lista.
void dlist_recorrer(DList, Visitante, void*);

// dlist_duplicar duplicar y gira una lista devolviendo el puntero al nodo 1.
DList dlist_duplicar_rotar (DList);

// dlist_destruir se encarga de liberar la memoria dedicada a la lista.
void dlist_destruir (DList, Visitante);

// dlist_intercambiar intercambia el dato de dos nodos de la estructura.
void dlist_intercambiar(DNodo*, DNodo*);

// dlist_split separa la lista doblemente enlazada en dos, devolviendo un
// puntero DNodo, el cual es el comienzo de la segunda.
DList dlist_split(DList);

// Funciones de ordenamiento:

// dlist_selectionSort: implementacion del algoritmo selection sort
// en listas doblemente enlazadas.
DList dlist_selectionSort(DList, Comparacion);

// dlist_insertionSort: implementacion del algoritmo insertion sort
// en listas doblemente enlazadas.
DList dlist_insertionSort(DList, Comparacion);

// dlist_merge: une dos listas cuyo primer elemento cumple una condicion
// Funcion auxiliar de mergeSort.
DList dlist_merge(DList, DList, Comparacion);

// dlist_mergeSort: implementacion del algoritmo merge sort en listas
DList dlist_mergeSort(DList, Comparacion);



#endif
