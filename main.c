#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  AVLTree A = itree_crear();
  AVLTree B = itree_crear();
  Intervalo *intervalo1 = malloc(sizeof(Intervalo));
  Intervalo *intervalo2 = malloc(sizeof(Intervalo));
  Intervalo *intervalo3 = malloc(sizeof(Intervalo));
  Intervalo *intervalo4 = malloc(sizeof(Intervalo));
  intervalo1->inicio = 1;
  intervalo2->inicio = 7;
  intervalo3->inicio = 14;
  intervalo4->inicio = 3;
  intervalo1->final = 2;
  intervalo2->final = 8;
  intervalo3->final = 15;
  intervalo4->final = 4;
  A = itree_insertar(A, intervalo1);
  A = itree_insertar(A, intervalo2);
  A = itree_insertar(A, intervalo3);
  B = itree_insertar(B, intervalo4);
  AVLTree C = conjuntoavl_union(A, B);
  printf("Conjunto union C\n");
  itree_recorrer_inorder(C, intervalo_imprimir);
  return 0;
}