#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "avltree/avltree.h"
#include "avltree/lists/queue.h"
#include "avltree/lists/stack.h"
#define CAPACIDAD 200


/*
int intervalo_verificar(char *inicio, char *final, Intervalo * intervalo) {
  char *errorI;
  double inicioD = strtod(inicio, &errorI);
  char *errorF;
  double finalD = strtod(final, &errorF);

  if (strcmp(errorI, "") != 0 || strcmp(errorF, "") != 0)
    return 0;
  if (finalD < inicioD)
    return 0;
  intervalo->inicio = inicioD;
  intervalo->final = finalD;
  return 1;
}

// error 1 = no corresponde a una funcion
// error 2 = intervalo invalido
// error 3 = existe residuo
char funcion_verificar(char *ident, char *inicio, char *final, char *residuo,
                       Intervalo * intervalo) {
  // Chequeamos si existe residuo luego del ultimo ]
  if (strcmp(residuo, "") != 0)
    return '3';
  // Si no hay residuo y el ident corresponde a un funcion con intervalo:
  if ((ident[0] == 'i' || ident[0] == 'e' || ident[0] == '?')
      && strlen(ident) == 2 && ident[1] == ' ') {
    // Verificamos que el intervalo sea valido:
    if (intervalo_verificar(inicio, final, intervalo))
      // Si es valido retorneamos el identificador
      return ident[0];
    // Sino codigo de error.
    else
      return '2';
  } else {
    // Si el identificador no corresponde a funciones con intervalo, 
    //comprobamos las restantes
    if (strcmp(ident, "dfs") == 0 || strcmp(ident, "bfs") == 0
        || strcmp(ident, "salir") == 0)
      return ident[0];
  }
  // No corresponde a ninguna funcion valida
  return '1';
}

void copiar_seccion(char *comando, char *parte, int i, int *cont,
                    int *indexToken, char eow) {
  if (comando[i] == eow) {
    (*indexToken)++;
    (*cont) = -1;
  } else
    parte[(*cont)] = comando[i];
}

char entrada_validar(char *comando, Intervalo * intervalo) {
  int i = 0, cont = 0;
  int indexToken = 0;
  char eows[] = "[,]";
  // Variables que almacenaran las distintas secciones del comando.
  char *ident = calloc(CAPACIDAD, sizeof(char));
  char *inicio = calloc(CAPACIDAD, sizeof(char));
  char *final = calloc(CAPACIDAD, sizeof(char));
  char *residuo = calloc(CAPACIDAD, sizeof(char));

  for (; comando[i] != '\n' && comando[i] != '\r'; i++, cont++) {
    // Copiamos residuo
    if (indexToken == 3)
      copiar_seccion(comando, residuo, i, &cont, &indexToken, '\n');
    // Copiamos final
    if (indexToken == 2)
      copiar_seccion(comando, final, i, &cont, &indexToken, eows[2]);
    // Copiamos inicio
    if (indexToken == 1)
      copiar_seccion(comando, inicio, i, &cont, &indexToken, eows[1]);
    // Copiamos el identificador hasta encontrar la llave.
    if (indexToken == 0)
      copiar_seccion(comando, ident, i, &cont, &indexToken, eows[0]);
  }
  char primeraLetra =
      funcion_verificar(ident, inicio, final, residuo, intervalo);
  free(ident);
  free(inicio);
  free(final);
  free(residuo);

  return primeraLetra;
}

int main() {

  int salida = 1;
  AVLTree arbol = itree_crear();

  printf("Interfaz 1.0\n");

  while (salida) {
    char *comando = malloc(sizeof(char) * CAPACIDAD);
    // leemos con \n incluido
    Intervalo *intervalo = malloc(sizeof(Intervalo));
    char identificador;

    fgets(comando, CAPACIDAD, stdin);
    // Si se excede la capacidad maxima queda caracteres en el buffer,
    // en ese caso limpiamos el buffer y notificamos el error.
    
    if (strlen(comando) == CAPACIDAD-1 ){
      scanf("%*[^\n]");
      scanf("%*c");
      identificador = '4'; 
    } else
      identificador = entrada_validar(comando, intervalo);
    
    free(comando);

    // Dependiendo del identificador la accion sera distinta.
    switch (identificador) {
    case 'i':
      arbol = itree_insertar(arbol, intervalo);
      break;

    case 'e':
      arbol = itree_eliminar(arbol, intervalo, 0);
      break;

    case '?':{
        AVLTree inter = itree_intersecar(arbol, intervalo);
        if (inter == NULL)
          printf("NO\n");
        else {
          printf("SI: ");
          intervalo_imprimir(inter->intervalo);
        }
        break;
      }
    case 'd':
      itree_recorrer_dfs(arbol, intervalo_imprimir);
      break;

    case 'b':
      itree_recorrer_bfs(arbol, intervalo_imprimir);
      break;

    case 's':
      printf("Saliendo del programa\n");
      salida = 0;
      break;

    case '1':
      printf("ERROR-Funcion invalida!\n");
      break;

    case '2':
      printf("ERROR-Intervalo invalido!\n");
      break;

    case '3':
      printf("ERROR-Caracteres irreconocibles tras ']'!\n");
      break;

    case '4':
      printf("ERROR-Maximo de caracteres excedido!\n");
      break;

    default:
      printf("ERROR-Caso desconocido, no debiste llegar aqui...");
      break;
    }
    // Si el intervalo no es insertado, o no es valido 
    //se libera ese espacio de memoria.
    if (identificador != 'i')
      free(intervalo);
  }
  // Se destruye el arbol sobre el cual se realizan las opearaciones.
  itree_destruir(arbol);

  return 0;
}


*/

int main() {
  // Pruebo union dis
  AVLTree A = itree_crear();
  Intervalo *intervalo1 = malloc(sizeof(Intervalo));
  Intervalo *intervalo2 = malloc(sizeof(Intervalo));
  Intervalo *intervalo3 = malloc(sizeof(Intervalo));
  intervalo1->inicio = 1;
  intervalo1->final = 2;
  intervalo2->inicio = 7;
  intervalo2->final = 8;
  intervalo3->inicio = 14;
  intervalo3->final = 15;
  A = itree_insertar(A, intervalo1);
  A = itree_insertar(A, intervalo2);
  A = itree_insertar(A, intervalo3);

  AVLTree B = itree_crear();
  Intervalo* intervalo4 = malloc(sizeof(Intervalo));
  intervalo4->inicio = 3;
  intervalo4->final = 4;
  B = itree_insertar(B, intervalo4);

  AVLTree D = itree_crear();
  Intervalo* intervalo5 = malloc(sizeof(Intervalo));
  intervalo5->inicio = 1;
  intervalo5->final = 7;
  D = itree_insertar(D, intervalo5);

  AVLTree I = itree_crear();
  Intervalo* intervalo6 = malloc(sizeof(Intervalo));
  intervalo6->inicio = -INT_MAX;
  intervalo6->final = INT_MAX;
  I = itree_insertar(I, intervalo6);

  AVLTree C = conjuntoavl_union(A, B);
  printf("Conjunto union A u B:\n");
  itree_imprimir(C, intervalo_imprimir);
  printf("\n");

  AVLTree E = conjuntoavl_union(A, D);
  printf("Conjunto union A u D:\n");
  itree_imprimir(E, intervalo_imprimir);
  printf("\n");

  AVLTree H = conjuntoavl_union(A, I);
  printf("Conjunto union A u I:\n");
  itree_imprimir(H, intervalo_imprimir);
  printf("\n");
  // ----------
  AVLTree F = conjuntoavl_interseccion(A, B);
  printf("Conjunto interseccion A i B:\n");
  itree_imprimir(F, intervalo_imprimir);
  printf("\n");

  AVLTree G = conjuntoavl_interseccion(A, D);
  printf("Conjunto interseccion A i D:\n");
  itree_imprimir(G, intervalo_imprimir);
  printf("\n");

  AVLTree J = conjuntoavl_interseccion(A, I);
  printf("Conjunto interseccion A i I:\n");
  itree_imprimir(J, intervalo_imprimir);
  printf("\n");
  // ----------
  AVLTree K = conjuntoavl_resta(A, B);
  printf("Conjunto resta A r B:\n");
  itree_imprimir(K, intervalo_imprimir);
  printf("\n");

  AVLTree L = conjuntoavl_resta(A, D);
  printf("Conjunto resta A r D:\n");
  itree_imprimir(L, intervalo_imprimir);
  printf("\n");

  AVLTree M = conjuntoavl_resta(A, I);
  printf("Conjunto resta A r I:\n");
  itree_imprimir(M, intervalo_imprimir);
  printf("\n");

  AVLTree N = conjuntoavl_resta(I, A);
  printf("Conjunto resta I r A:\n");
  itree_imprimir(N, intervalo_imprimir);
  printf("\n");
  // ----------
  AVLTree O = conjuntoavl_complemento(A);
  printf("Conjunto complemento A:\n");
  itree_imprimir(O, intervalo_imprimir);
  printf("\n");

  AVLTree V = itree_crear();
  Intervalo* intervalo7 = malloc(sizeof(Intervalo));
  intervalo7->inicio = INVALINI;
  intervalo7->final = INVALFIN;
  V = itree_insertar(V, intervalo7);

  AVLTree P = conjuntoavl_complemento(V);
  printf("Conjunto complemento vacio:\n");
  itree_imprimir(P, intervalo_imprimir);
  printf("\n");

  AVLTree Q = conjuntoavl_complemento(I);
  printf("Conjunto complemento infinito:\n");
  itree_imprimir(Q, intervalo_imprimir);
  printf("\n");

  AVLTree Ir = itree_crear();
  Intervalo* intervalo8 = malloc(sizeof(Intervalo));
  intervalo8->inicio = -INT_MAX;
  intervalo8->final = 0;
  Intervalo* intervalo9 = malloc(sizeof(Intervalo));
  intervalo9->inicio = 1;
  intervalo9->final = INT_MAX;
  Ir = itree_insertar(Ir, intervalo8);
  Ir = itree_insertar(Ir, intervalo9);

  AVLTree R = conjuntoavl_complemento(Ir);
  printf("Conjunto complemento infinito compuesto:\n");
  itree_imprimir(R, intervalo_imprimir);
  printf("\n");

  itree_destruir(A);
  itree_destruir(B);
  itree_destruir(C);
  itree_destruir(D);
  itree_destruir(E);
  itree_destruir(F);
  itree_destruir(G);
  itree_destruir(H);
  itree_destruir(I);
  itree_destruir(J);
  itree_destruir(K);
  itree_destruir(L);
  itree_destruir(M);
  itree_destruir(N);
  itree_destruir(O);
  itree_destruir(P);
  itree_destruir(Q);
  itree_destruir(R);
  itree_destruir(V);
  itree_destruir(Ir);
  
  return 0;
}