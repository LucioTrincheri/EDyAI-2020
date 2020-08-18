#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "avltree.h"
#include "lists/queue.h"
#include "lists/stack.h"

// Funciones auxiliares -------------------------------------------------------


int min(int a, int b) {
  if (a < b)
    return a;
  return b;
}

int max(int a, int b) {
  if (a > b)
    return a;
  return b;
}

int obtener_altura(AVLTree arbol) {
  if (arbol)
    return arbol->altura;
  return 0;
}

int obtener_balance(AVLTree arbol) {
  if (arbol)
    return obtener_altura(arbol->der) - obtener_altura(arbol->izq);
  return 0;
}

int obtener_mayorFinal(AVLTree arbol) {
  if (arbol->der == NULL || arbol->izq == NULL) {
    AVLTree temp = arbol->izq ? arbol->izq : arbol->der;
    // Sin hijos
    if (temp == NULL)
      return arbol->intervalo->final;
    else                        // Caso un hijo
      return max(arbol->intervalo->final, temp->mayorFinal);
  } else                        // Caso dos hijos
    return max(arbol->intervalo->final,
                      max(arbol->izq->mayorFinal,
                                 arbol->der->mayorFinal));
}

// Funciones de rotacion y balanceo. ------------------------------------------

AVLTree rotar_derecha(AVLTree arbol) {
  // Reservamos el izq del arbol
  AVLTree copArbolIzq = arbol->izq;
  // Realizamos la rotacion perse
  arbol->izq = copArbolIzq->der;
  copArbolIzq->der = arbol;
  // Recalculamos altura
  arbol->altura =
      max(obtener_altura(arbol->izq), obtener_altura(arbol->der)) + 1;
  arbol->mayorFinal = obtener_mayorFinal(arbol);
  copArbolIzq->altura =
      max(obtener_altura(copArbolIzq->izq),
          obtener_altura(copArbolIzq->der)) + 1;
  copArbolIzq->mayorFinal = obtener_mayorFinal(copArbolIzq);
  // Se retorna el nuevo primer nodoz
  return copArbolIzq;
}

AVLTree rotar_izquierda(AVLTree arbol) {
  // Reservamos el der del arbol
  AVLTree copArbolDer = arbol->der;
  // Realizamos la rotacion perse
  arbol->der = copArbolDer->izq;
  copArbolDer->izq = arbol;
  // Recalculamos altura
  arbol->altura =
      max(obtener_altura(arbol->izq), obtener_altura(arbol->der)) + 1;
  arbol->mayorFinal = obtener_mayorFinal(arbol);
  copArbolDer->altura =
      max(obtener_altura(copArbolDer->izq),
          obtener_altura(copArbolDer->der)) + 1;
  copArbolDer->mayorFinal = obtener_mayorFinal(copArbolDer);
  // Se retorna el nuevo primer nodo
  return copArbolDer;
}

// Segun el valor del balance del nodo se decide como rebalancearlo.
AVLTree balancear(AVLTree arbol, int balance) {
  //Izquierda Izquierda
  if (balance < -1 && obtener_balance(arbol->izq) <= 0)
    return rotar_derecha(arbol);
  // Izquierda derecha
  if (balance < -1 && obtener_balance(arbol->izq) > 0) {
    arbol->izq = rotar_izquierda(arbol->izq);
    return rotar_derecha(arbol);
  }
  // Derecha derecha
  if (balance > 1 && obtener_balance(arbol->der) >= 0)
    return rotar_izquierda(arbol);
  // Derecha izquieda
  if (balance > 1 && obtener_balance(arbol->der) < 0) {
    arbol->der = rotar_derecha(arbol->der);
    return rotar_izquierda(arbol);
  }
  // Arbol balanceado
  return arbol;
}

// Funciones creacion ---------------------------------------------------------

AVLTree itree_crear() {
  return NULL;
}

AVLTree inodo_crear(Intervalo * dato) {
  AVLTree nodo = malloc(sizeof(struct _AVLNodo));
  nodo->der = NULL;
  nodo->izq = NULL;
  nodo->altura = 1;
  nodo->mayorFinal = dato->final;
  nodo->intervalo = dato;
  return nodo;
}

AVLTree itree_insertar(AVLTree arbol, Intervalo * dato) {
  // Si llegamos a nodo vacio insertamos nuestro nodo nuevo
  if (arbol == NULL) {
    return inodo_crear(dato);
  }
  // Buscamos la posicion que debe ocupar el nuevo nodo
  // segun BST, teniendo en cuenta ambos valores del intervalos.
  if (dato->inicio < arbol->intervalo->inicio)
    arbol->izq = itree_insertar(arbol->izq, dato);
  else if (dato->inicio > arbol->intervalo->inicio)
    arbol->der = itree_insertar(arbol->der, dato);
  else if (dato->final < arbol->intervalo->final)
    arbol->izq = itree_insertar(arbol->izq, dato);
  else if (dato->final > arbol->intervalo->final)
    arbol->der = itree_insertar(arbol->der, dato);
  // Caso en el que el nodo ya este en el arbol.
  else
    return arbol;
  // Recalculamos la altura de cada nuevo nodo.
  arbol->altura =
      1 + max(obtener_altura(arbol->izq), obtener_altura(arbol->der));
  arbol->mayorFinal = obtener_mayorFinal(arbol);
  // Calculamos el valor de balance de el nodo y procedemos a balancear.
  int balance = obtener_balance(arbol);
  return balancear(arbol, balance);
}

// Funciones comparacion. -----------------------------------------------------

int inodo_interseccion(Intervalo * datoArbol, Intervalo * dato) {
  if (datoArbol->inicio <= dato->final && datoArbol->final >= dato->inicio)
    return 1;
  return 0;
}

AVLTree itree_intersecar(AVLTree arbol, Intervalo * dato) {
  // Si llegue a NULL, retorno
  if (arbol == NULL)
    return arbol;
  // Si se encontro interseccion, devuelvo ese nodo 
  if (inodo_interseccion(arbol->intervalo, dato))
    return arbol;
  // si el intervalo es menor que el del nodo, obligatoriamente la
  // interseccion ocurrira en el subarbol izquierdo

  // si el dato es menor al nodo:
  if (dato->final < arbol->intervalo->inicio) {
    // Si el mayor de los descendientes de la izquierda es mayor que el dato,
    // hago recursion ya que puede ser que haya interseccion en el mismo
    if (arbol->izq != NULL && arbol->izq->mayorFinal >= dato->inicio)
      return itree_intersecar(arbol->izq, dato);
    // Si es menor, no hay forma que haya interseccion.
    else
      return NULL;
  }
  // si el dato es mayor al nodo:
  // Si el mayor de los descendientes de la izquierda es mayor que el dato,
  // se que si o si hay una interseccion a la izquierda, asi que la busco 
  if (arbol->izq != NULL && arbol->izq->mayorFinal >= dato->inicio)
    return itree_intersecar(arbol->izq, dato);
  // En caso contrario, la interseccion puede llegar 
  // a ocurrir a la derecha solamente
  else
    // Si el mayor del arbol es menor que el dato
    // ya se que no habra interseccion
  if (arbol->mayorFinal < dato->inicio)
    return NULL;
  // sino, finalmente hago recursion por la derecha
  else
    return itree_intersecar(arbol->der, dato);
}

// Funciones destrucción. -----------------------------------------------------

void inodo_liberar(AVLTree arbol) {
  free(arbol->intervalo);
  free(arbol);
}

AVLTree itree_eliminar(AVLTree arbol, Intervalo * dato, int bandera) {
  if (arbol == NULL) {
    printf("No existe el nodo a destruir\n");
    return arbol;
  }
  // Se hubica el nodo a eliminar.
  if (dato->inicio < arbol->intervalo->inicio)
    arbol->izq = itree_eliminar(arbol->izq, dato, bandera);
  else if (dato->inicio > arbol->intervalo->inicio)
    arbol->der = itree_eliminar(arbol->der, dato, bandera);
  else if (dato->final < arbol->intervalo->final)
    arbol->izq = itree_eliminar(arbol->izq, dato, bandera);
  else if (dato->final > arbol->intervalo->final)
    arbol->der = itree_eliminar(arbol->der, dato, bandera);
  else {
    if (arbol->der == NULL || arbol->izq == NULL) {
      AVLTree temp = arbol->izq ? arbol->izq : arbol->der;
      // Sin hijos
      if (temp == NULL) {
        temp = arbol;
        // Caso directo, se libera todo
        if (bandera == 0)
          inodo_liberar(temp);
        // Caso venir de dos hijos. No se libera el intervalo
        if (bandera == 1)
          free(temp);
        arbol = NULL;
      } else {// Caso un hijo
        // Caso directo, se libera intervalo de raiz, compia datos hijo
        // libera nodo hijo sin liberar intervalo
        if (bandera == 0) {
          free(arbol->intervalo);
          arbol->intervalo = temp->intervalo;
          arbol->izq = temp->izq;
          arbol->der = temp->der;
          free(temp);
        }
        // Caso venir de dos hijos, igual anterior no se libera intervalo raiz
        // ya que ahora es la raiz de la llamada original de dos hijos.
        // swap
        if (bandera == 1) {
          arbol->intervalo = temp->intervalo;
          arbol->izq = temp->izq;
          arbol->der = temp->der;
          free(temp);
        }
      }
    } else {                    // Caso dos hijos
      // Busco el nodo menor del hijo derecho
      AVLTree actual = arbol->der;
      while (actual->izq != NULL)
        actual = actual->izq;
      // Copio los datos del nodo encontrado y borro ese nodo
      free(arbol->intervalo);
      arbol->intervalo = actual->intervalo;
      // bandera = 1 para no eliminar el intervalo al encontrar "actual"
      arbol->der = itree_eliminar(arbol->der, actual->intervalo, 1);
    }
  }
  // Si no quedan nodos tras la eliminacion (caso inicial no hijos), retornamos
  if (arbol == NULL)
    return arbol;
  // Actualizo la altura si es necesaria
  arbol->altura =
      1 + max(obtener_altura(arbol->der), obtener_altura(arbol->izq));
  arbol->mayorFinal = obtener_mayorFinal(arbol);
  // Calculo el balance del nodo
  int balance = obtener_balance(arbol);

  // Retorno el arbol tras balancearlo
  return balancear(arbol, balance);
}

void itree_destruir(AVLTree arbol) {
  if (arbol != NULL) {
    itree_destruir(arbol->der);
    itree_destruir(arbol->izq);
    inodo_liberar(arbol);
  }
}

// ----------------------------- CONJUNTOS AVL ---------------------------- //

// Funciones recorrido y aplicacion modificadas //

int intervalo_inval(Intervalo* intervalo){
  if(intervalo->inicio == INVALINI && intervalo->final == INVALFIN)
    return 1;
  return 0;
}

// Modifico dfs del tp anterior. Lo uso para aplicar la funcion 
// visitante a los nodos del primer arbol sobre el segundo arbol.
// Casos de esto incluye union e interseccion.

// Para aplicar funciones visitantes a un solo arbol, uso inorder. 
// Ejemplos de esto es imprimir

// Debido a la falta de necesidad, se borro itree_recorrer_bfs.

AVLTree itree_recorrer_dfs(AVLTree arbol, VisitanteArboles visitante, AVLTree objetivo) {
  Stack nodos = stack_new();
  // Corrijo error sobre el tp2, donde pusheabamos nulls al stack
  if(arbol)
    stack_push(nodos, arbol);
  while (!stack_isEmpty(nodos)) {
    AVLTree nodo = stack_top(nodos);
    stack_pop(nodos);
    objetivo = visitante(nodo->intervalo, objetivo);
    // Solo pusheo los hijos si no son null (problema de la entrega anterior).
    if(nodo->der)
      stack_push(nodos, nodo->der);
    if(nodo->izq)
      stack_push(nodos, nodo->izq);
    }
    stack_destruir(nodos);
    return objetivo;
  }

// Tambien modifico inorden, solo que esta se usa en el caso de aplicar
// la funcion al intervalo en si y no sobre un arbol auxiliar objetivo.
void itree_recorrer_inorder(AVLTree arbol, Visitante visitante) {
  if (arbol == NULL)
    return;
  itree_recorrer_inorder(arbol->izq, visitante);
  visitante(arbol->intervalo);
  itree_recorrer_inorder(arbol->der, visitante);
}


// Funciones visitante modificadas.

void intervalo_imprimir(Intervalo * intervalo) {
  if(intervalo->inicio == intervalo->final)
    printf("%d", intervalo->inicio);
  else {
    if(intervalo_inval(intervalo))
      printf("VACIO"); // Ø
    else if(intervalo->inicio == -INT_MAX){
      if(intervalo->final == INT_MAX)
        printf("-INF:INF");
      else
        printf("-INF:%d", intervalo->final);
    } else if(intervalo->final == INT_MAX)
      printf("%d:INF", intervalo->inicio);
    else 
      printf("%d:%d", intervalo->inicio, intervalo->final);
  }
}

// TODO Terminar imprimir y borrar irorder (obsoleta) 

void itree_imprimir(AVLTree arbol, Visitante visitante){
  if(!arbol){
    printf("No existe un conjunto asociado a ese alias\n");
    return;
  }
  Stack nodos = stack_new();
  AVLTree temp = arbol;
  while (temp != NULL || !stack_isEmpty(nodos)) {
    while (temp != NULL) {
      stack_push(nodos, temp);
      temp = temp->izq;
    }
    temp = stack_top(nodos);
    stack_pop(nodos);
    
    visitante(temp->intervalo);

    temp = temp->der;
    if (temp != NULL || !stack_isEmpty(nodos))
      printf(",");
  }
  stack_destruir(nodos);
}


// Nuevas funciones de insercion / creacion.

AVLTree itree_duplicar(AVLTree arbol){
  if(!arbol)
    return NULL;
  
  AVLTree duplicado = malloc(sizeof(struct _AVLNodo));
  duplicado->altura = arbol->altura;
  duplicado->mayorFinal = arbol->mayorFinal;
  duplicado->intervalo = malloc(sizeof(Intervalo));
  duplicado->intervalo->inicio = arbol->intervalo->inicio;
  duplicado->intervalo->final = arbol->intervalo->final;
  
  duplicado->der = itree_duplicar(arbol->der);
  duplicado->izq = itree_duplicar(arbol->izq);
  
  return duplicado;
}


Intervalo* intervalo_copiar(Intervalo* copia, Intervalo* intervalo){
  copia->inicio = intervalo->inicio;
  copia->final = intervalo->final;
  return copia;
}
Intervalo* intervalo_expandir(Intervalo* expandido){
  if(expandido->inicio != -INT_MAX)
    expandido->inicio -=1;
  if(expandido->final != INT_MAX)
    expandido->final +=1;
  return expandido;
}

// Inserta un intervalo en un arbol argumento, de forma que
// los intervalos del arbol final sean disjuntos entre si. // !! TODO COMENTAR
AVLTree itree_insertar_disyuncion(Intervalo* intervalo, AVLTree arbol){
  if (intervalo->inicio == INVALINI && intervalo->final == INVALFIN)
    return arbol;
  Intervalo* copia = malloc(sizeof(Intervalo));
  copia = intervalo_copiar(copia, intervalo);
  if(!arbol)
    return itree_insertar(arbol, copia);
  Intervalo* expandido = malloc(sizeof(Intervalo));
  expandido = intervalo_copiar(expandido, intervalo);
  expandido = intervalo_expandir(expandido);
  // Creo un nuevo intervalo expandido en 1 en cada extremo.
  // Esto lo hago para comprobar la interseccion entre el arbol
  // y el intervalo original
  // Por ejemplo si el intervalo fuese (3,4) y el arbol contiene al (5,6)
  // la interseccion daria vacia, aunque la union pudiese dar el conjunto (3,6)
  AVLTree interseccionExp = itree_intersecar(arbol, expandido);
  while(interseccionExp){
    copia->inicio = min(copia->inicio, interseccionExp->intervalo->inicio);
    copia->final = max(copia->final, interseccionExp->intervalo->final);
    arbol = itree_eliminar(arbol, interseccionExp->intervalo, 0);

    expandido = intervalo_copiar(expandido, copia);
    expandido = intervalo_expandir(expandido);

    interseccionExp = itree_intersecar(arbol, expandido);
  }
  free(expandido);
  free(interseccionExp); // ! puede estar al pedo ya que es null (checkear memoria)

  arbol = itree_insertar(arbol, copia);
  return arbol;

}

// Funciones de conjuntos
AVLTree conjuntoavl_union(AVLTree conjA, AVLTree conjB){
  if(intervalo_inval(conjA->intervalo))
    return itree_duplicar(conjB);
  if(intervalo_inval(conjB->intervalo))
    return itree_duplicar(conjA);
  // Intersecar hace un check de un intervalo contra un arbol hasta
  // que el primero sea vacio. Debido a esto, es menos costoso
  // intersecar los nodos del arbol mas chico al arbol mas grande
  AVLTree duplicado = itree_crear();
  if(conjA->altura <= conjB->altura){
    duplicado = itree_duplicar(conjB);
    duplicado = itree_recorrer_dfs(conjA, itree_insertar_disyuncion, duplicado);
  }
  else{
    duplicado = itree_duplicar(conjA);
    duplicado = itree_recorrer_dfs(conjB, itree_insertar_disyuncion, duplicado);
  }
  return duplicado;
}

Intervalo* intervalo_intersecado(Intervalo* inter1, Intervalo* inter2, Intervalo* resultado){
  resultado->inicio = max(inter1->inicio, inter2->inicio);
  resultado->final = min(inter1->final, inter2->final);
  return resultado;
}

AVLTree itree_agregar_interseccion(Intervalo* intervalo, AVLTree intersecado, AVLTree interseccion){
  AVLTree intersecta = itree_intersecar(intersecado, intervalo);
  if(intersecta){
    Intervalo* resultado = malloc(sizeof(Intervalo));
    resultado = intervalo_intersecado(intervalo, intersecta->intervalo, resultado);
    interseccion = itree_insertar_disyuncion(resultado, interseccion);
    free(resultado);
    if(intersecta->der)
      interseccion = itree_agregar_interseccion(intervalo, intersecta->der, interseccion);
    if(intersecta->izq)
      interseccion = itree_agregar_interseccion(intervalo, intersecta->izq, interseccion);
  }
  return interseccion;
}

AVLTree itree_interseccion_aux(AVLTree arbol, AVLTree intersecado, AVLTree interseccion){
  Stack nodos = stack_new();
  if(arbol)
    stack_push(nodos, arbol);
  while (!stack_isEmpty(nodos)) {
    AVLTree nodo = stack_top(nodos);
    stack_pop(nodos);
    interseccion = itree_agregar_interseccion(nodo->intervalo, intersecado, interseccion);
    if(nodo->der)
      stack_push(nodos, nodo->der);
    if(nodo->izq)
      stack_push(nodos, nodo->izq);
    }
  stack_destruir(nodos);
  return interseccion;
}

AVLTree conjuntoavl_interseccion(AVLTree conjA, AVLTree conjB){ // TODO checkear interseccion
  AVLTree interseccion = itree_crear();
  if(intervalo_inval(conjA->intervalo) || intervalo_inval(conjB->intervalo)){
    Intervalo* vacio = malloc(sizeof(Intervalo));
    vacio->inicio = INVALINI;
    vacio->final = INVALFIN;
    return itree_insertar(interseccion, vacio);
  }

  // Al contrario que union, como interseccion va eliminando elementos del
  // arbol de intervalos, es preferible duplicar el arbol mas pequeño para
  // tener que hacer menos intersecciones en el caso de que la interseccion
  // sea completa (todo el arbol menor esta dentro del arbol mayor)
  if(conjA->altura <= conjB->altura){
    interseccion = itree_interseccion_aux(conjB, conjA, interseccion);
  }
  else{
    interseccion = itree_interseccion_aux(conjA, conjB, interseccion);
  }

  if(!interseccion){
    Intervalo* vacio = malloc(sizeof(Intervalo));
    vacio->inicio = INVALINI;
    vacio->final = INVALFIN;
    interseccion = itree_insertar(interseccion, vacio);
  }
  return interseccion;
}

AVLTree itree_resta_intervalo(Intervalo* intervalo, AVLTree resta){
  if (intervalo->inicio == INVALINI && intervalo->final == INVALFIN)
    return resta;
  AVLTree interseccion = itree_intersecar(resta, intervalo);
  while(interseccion){
    if(intervalo->inicio <= interseccion->intervalo->inicio){
      if(intervalo->final < interseccion->intervalo->final){
        interseccion->intervalo->inicio = intervalo->final + 1;
        interseccion = itree_intersecar(resta, intervalo);
      } else {
        // Caso intervalo del arbol dentro del intervalo
        resta = itree_eliminar(resta, interseccion->intervalo, 0);
        interseccion = itree_intersecar(resta, intervalo);
      }
    } else {
      if(intervalo->final < interseccion->intervalo->final){
        // Caso intervalo dentro del intervalo del arbol
        Intervalo* resto = malloc(sizeof(Intervalo));
        resto->inicio = intervalo->final + 1;
        resto->final = interseccion->intervalo->final;
        interseccion->intervalo->final = intervalo->inicio - 1;
        resta = itree_insertar(resta, resto);
        return resta;
      } else {
        interseccion->intervalo->final = intervalo->inicio - 1;
        interseccion = itree_intersecar(resta, intervalo);
      }
    }
  }
  free(interseccion); // ! Al igual que agregar puede llegar a ser inutil checkear memoria
  return resta;
}

AVLTree conjuntoavl_resta(AVLTree conjA, AVLTree conjB){
  AVLTree resta = itree_crear();
  resta = itree_duplicar(conjA);
  if(intervalo_inval(conjA->intervalo) || intervalo_inval(conjB->intervalo))
    return resta;
  resta = itree_recorrer_dfs(conjB, itree_resta_intervalo, resta);
  if(!resta){
    Intervalo* vacio = malloc(sizeof(Intervalo));
    vacio->inicio = INVALINI;
    vacio->final = INVALFIN;
    resta = itree_insertar(resta, vacio);
  }
  return resta;
}

AVLTree conjuntoavl_complemento(AVLTree conjA){
  AVLTree universo = itree_crear();
  Intervalo* univ = malloc(sizeof(Intervalo));
  univ->inicio = -INT_MAX;
  univ->final = INT_MAX;
  universo = itree_insertar(universo, univ);
  AVLTree complemento = conjuntoavl_resta(universo, conjA);
  itree_destruir(universo);
  return complemento;
}