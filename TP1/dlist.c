#include <stdio.h>
#include <stdlib.h>
#include "cabeceras/dlist.h"

struct _DNodo {
  void* dato;
  struct _DNodo* sig, * ant;
};

DList dlist_crear() {
  return NULL;
}

DNodo* dnodo_crear(void* dato, DNodo*ant, DNodo*sig) {
  // se pide memoria para la estructura nodo.
  DNodo* nuevo = malloc(sizeof(DNodo));
  // se establecen las variables.
  nuevo->ant = ant;
  nuevo->sig = sig;
  nuevo->dato = dato;
  return nuevo;
}

DList dnodo_agregar_inicio(DList lista, void* dato) {
  if(lista == NULL) {
    // Caso lista vacia.
    DNodo* nuevo = dnodo_crear(dato, NULL, NULL);
    return nuevo;
  } else {
    // Caso lista con elementos.
    // Creo un nuevo nodo
    DNodo* nuevo = dnodo_crear(dato, NULL, lista);
    // El antiguo primer elemento ahora es el segundo, asi que le asigno su ant.
    lista->ant = nuevo;
    return nuevo;
  }
}

// Agregado aux, para dar mas posibilidades a la funcion visitante
// (ejemplo: contar cantidad, imprimir en un archivo).
void dlist_recorrer (DList lista, Visitante visitante, void * aux) {
  DNodo* Nsig = lista;
  for (;Nsig != NULL; Nsig = Nsig->sig) {
    visitante(Nsig->dato, aux);
  }
}

DList dlist_duplicar_rotar (DList lista) {
  DList nueva = dlist_crear();
  DNodo* Nsig = lista;
  for (; Nsig != NULL; Nsig = Nsig->sig) {
    nueva = dnodo_agregar_inicio(nueva, Nsig->dato);
  }
  return nueva;
}


void dlist_destruir (DList lista, Visitante liberar) {
  DList proximo = lista;
  DList actual;
  for (; proximo != NULL ; ) {
    // Guardo un puntero al nodo actual, me muevo al siguiente y libero.
    actual = proximo;
    proximo = proximo->sig;
    if (liberar != NULL)
      liberar(actual->dato, NULL);
    free(actual);
  }
}

void dlist_intercambiar(DNodo* nodoA, DNodo* nodoB) {
  // Intercambio los punteros de los nodos que contienen los datos.
  void* nodoC = nodoB->dato;
  nodoB->dato = nodoA->dato;
  nodoA->dato = nodoC;
}

DList dlist_selectionSort(DList lista, Comparacion c) {
  DNodo* nodo = lista;
  for (;nodo->sig != NULL; nodo = nodo->sig) {
    // Guardo en aux el nodo que tenga el dato menor segun la comparacion.
    DNodo* aux = nodo;
    // Itero sobre el siguiente elemento de la lista
    DNodo* nodo2 = nodo->sig;
    for (;nodo2 != NULL; nodo2 = nodo2->sig) {
      // Si el nodo sobre el que se itera es menor al guardado, lo guardo.
      if (c(nodo2->dato, aux->dato))
        aux = nodo2;
    }
    // Si hubo cambios en el nodo auxiliar, entonces intercambio los valores.
    if (aux != nodo){
      dlist_intercambiar(nodo, aux);
    }
  }
  return lista;
}

DList dlist_insertionSort(DList lista, Comparacion c) {
  DNodo* nodo, * nodo2;
  for (nodo = lista->sig; nodo != NULL; nodo = nodo->sig) {
    void* dato = nodo->dato;
    // Recorro los nodos hacia atras hasta encontrar
    // uno con su dato menor al del dato guardado
    for (nodo2 = nodo->ant; nodo2 && !c(nodo2->dato, dato); nodo2 = nodo2->ant)
      // Muevo el valor de nodo2 un lugar hacia atras ya que es mayor.
      nodo2->sig->dato = nodo2->dato;
    // Ahora debemos colocar el dato es su posicion.
    // En el caso que el for anterior haya recorrido hasta nodo2=NULL, el dato
    // sera colocado en la 1ra posicion de la lista.
    // En caso contrario, se coloca en su posicion adecuada.
    if (!nodo2)
      lista->dato = dato;
    else
      nodo2->sig->dato = dato;
  }
  return lista;
}

DList dlist_split(DList primero) {
  // Separo la lista a la mitad mediante el metodo de puntero lento/rapido
  DList rapido = primero;
  DList lento = primero;
  while (rapido->sig && rapido->sig->sig) {
    rapido = rapido->sig->sig;
    lento = lento->sig;
  }
  DList aux = lento->sig;
  lento->sig = NULL;
  aux->ant = NULL;
  return aux;
}


DList dlist_merge(DList lista1, DList lista2, Comparacion c){
  // Si alguna de las listas en NULL, retorno
  if (lista1 == NULL)
    return lista2;
  if (lista2 == NULL)
    return lista1;
  // Declaro / inicializo variables auxiliares:
  // punteroAlInicio: apunta siempre al 1er nodo de la lista (retorno de merge)
  DList punteroAlInicio = lista1, lista2Aux, lista1Aux = lista1;

  while (lista1 != NULL && lista2 != NULL) {
    // Si el dato de la primera lista es menor al dato de la segunda lista:
    if (c(lista1->dato, lista2->dato)) {
      // En el caso que sea el ultimo nodo de la primera lista, lo guardamos
      if (lista1->sig == NULL)
        lista1Aux = lista1;
      // Avanzo al siguiente nodo de la primera lista
      lista1 = lista1->sig;
    // En caso contrario
    } else {
      // Agregamos el nodo de la segunda lista adelante del nodo de la primera
      lista2Aux = lista2->sig;
      if (lista1->ant == NULL)
        // Si el nodo de la primera lista es el primero,
        // actualizamos el puntero al inicio de la lista a retornar
        punteroAlInicio = lista2;
      lista2->ant = lista1->ant;
      lista2->sig = lista1;
      if (lista1->ant != NULL)
        lista1->ant->sig = lista2;
      lista1->ant = lista2;
      lista2 = lista2Aux;
    }
  }

  // En el caso que la segunda lista aun contenga nodos
  if (lista2 != NULL) {
    // Utilizamos el lista1Aux guardado anteriormente
    //para que apunte al resto de la segunda lista
    lista1Aux->sig = lista2;
    lista2->ant = lista1Aux;
  }

  return punteroAlInicio;
}

DList dlist_mergeSort(DList primero, Comparacion c){
  // Si la lista tiene 0 o 1 elementos retorno, ya que la lista esta ordenada
  if (primero==NULL || primero->sig == NULL)
    return primero;
  // Separo la lista y guardo el nodo apuntando al comienzo de la segunda lista
  DList splitMedio = dlist_split(primero);

  // Llamo mergeSort recursivamente sobre ambas listas
  primero = dlist_mergeSort(primero, c);
  splitMedio = dlist_mergeSort(splitMedio, c);

  // Hago un merge de las listas
  return dlist_merge(primero, splitMedio, c);
}
