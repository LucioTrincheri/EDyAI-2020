#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

// Ya que como no puedo liberar el puntero intervalo al restar intervalos,
// represento el intervalo vacio como un intervalo de valores invalidos.
#define INVALINI 1
#define INVALFIN -1

typedef struct _Intervalo {
    double inicio;
    double final;
}Intervalo;

typedef struct _AVLNodo {
    Intervalo *intervalo;
    int altura;
    double mayorFinal;
    struct _AVLNodo *der;
    struct _AVLNodo *izq;
}*AVLTree;


// Modifico visitante sobre el tp anterior. Ahora ademas de un intervalo,
// toma un AVLTree al cual aplicar la función (en caso de usarse por dfs)
// y devulve el arbol modificado resultante de esta operacion.
typedef AVLTree (*Visitante) (Intervalo *, AVLTree);

// itree_crear inicializa el nuevo arbol de intervalos.
AVLTree itree_crear();

// itree_destruir libera toda la memoria utilizada en el arbol.
void itree_destruir(AVLTree);

// itree_insertar inserta un nuevo intervalo en el arbol, 
//manteniendo el balance.
AVLTree itree_insertar(AVLTree, Intervalo *);

// itree_intersecar busca en el arbol si un intervalo dado tiene
//interseccion con algun nodo.
AVLTree itree_intersecar(AVLTree, Intervalo *);

// itree_eliminar elimina el nodo que contenga el intervalo dado.
AVLTree itree_eliminar(AVLTree, Intervalo *, int);

// itree_recorrer_dfs recorre el arbol de la forma dfs, aplicando la 
// funcion visitante a cada nodo del primer arbol sobre el segundo arbol.
void itree_recorrer_dfs(AVLTree, Visitante, AVLTree);

// itree_recorrer_dfs recorre el arbol de la forma bfs. Sin uso actual.
void itree_recorrer_bfs(AVLTree, Visitante);

// intervalo_imprimir (visitante) imprime los valores de un intervalo dado.
void intervalo_imprimir(Intervalo *);

#endif