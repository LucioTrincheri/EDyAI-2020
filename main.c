#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "hash/hash.h"
#include "avltree/avltree.h"
#include "avltree/lists/queue.h"
#include "avltree/lists/stack.h"
#define LARGO 256
#define CAPACIDAD 1000
#define OPERACION 1
#define EXTENSION 2
#define COMPRENSION 3
#define EXCESO 4
#define ERROR 5
#define IMPRIMIR 6
#define SALIR 7


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


char funcion_verificar(char *inicio, char *igual, char *final, int leidos) {
  if(strcmp(inicio, "salir") == 0){
    if (leidos == 1)
      return SALIR;
  } else if(strcmp(inicio, "imprimir") == 0){
    if(leidos == 2)
      return IMPRIMIR;
  } else if(strcmp(igual, "=") == 0){
    if(leidos == 3){
      if(final[0] == '{')
        if(strncmp(final, "{x :", 4) == 0) // Modificar con strchr
          return COMPRENSION;
        else
          return EXTENSION;
      else
        return OPERACION;
    }
  }
  return ERROR;
}



void insertar_extension(Hash* hash, char* alias, char* conjunto){
  char* numeros = calloc(LARGO, sizeof(char));
  char* resto = calloc(LARGO, sizeof(char));
  char end;
  sscanf(conjunto, "{%255[^}.]} %255[^\n]%c", numeros, resto, &end);
  const char ch = '.';
  if(end != '\0' || strchr(numeros, ch) != NULL){
    printf("Error del conjunto por extension\n");
    free(numeros);
    free(resto);
    return;
  }
  AVLTree arbol = itree_crear();
  if(strlen(numeros) == 0) {
    Intervalo* vacio = malloc(sizeof(Intervalo));
    vacio->inicio = INVALINI;
    vacio->final = INVALFIN;
    arbol = itree_insertar(arbol, vacio);
    itree_imprimir(arbol, intervalo_imprimir);
    hash_insertar(hash, alias, arbol);
    free(numeros);
    free(resto);
    return;
  }
  const char separador[2] = ",";
  char* token = strtok(numeros, separador);
  // Numeros: 1,2,3,4,5,6
  while(token != NULL){
    Intervalo* valor = malloc(sizeof(Intervalo));
    long numero = strtol(token, NULL, 10);
    if(numero >= -INT_MAX && numero<= INT_MAX){
      valor->inicio = numero;
      valor->final = numero;
      arbol = itree_insertar_disyuncion(valor, arbol);
      free(valor);
    } else {
      itree_destruir(arbol);
      printf("Intervalo fuera del rango de los enteros");
      free(numeros);
      free(resto);
      //! Puede ser necesario freear token
      return;
    }
    token = strtok(NULL, separador);
  }
  free(numeros);
  free(resto);
  hash_insertar(hash, alias, arbol);
}

void insertar_comprension(Hash* hash, char* alias, char* conjunto){
  char var1, var2, num1[20], num2[20];
  char* resto = calloc(LARGO, sizeof(char));
  char end;
  const char ch = '.';
  if(strchr(conjunto, ch) != NULL){
    printf("Se encontraron numeros de coma flotante\n");
    free(resto);
    return;
  }
  sscanf(conjunto, "{%c : %19s <= %c <= %19[^}\n]} %255[^\n]%c", &var1, num1, &var2, num2, resto, &end);
  if(end != '\0' || var1 != var2 || var1 != 'x'){
    printf("Conjunto por comprension mal desarrollado\n");
    free(resto);
    return;
  }
  long numero1, numero2;
  if(strcmp(num1, "-INF") == 0)
    numero1 = -INT_MAX;
  else if (strcmp(num1, "INF") == 0)
    numero1 = INT_MAX;
  else
    numero1 = strtol(num1, NULL, 10);

  if(strcmp(num2, "-INF") == 0)
    numero2 = -INT_MAX;
  else if (strcmp(num2, "INF") == 0)
    numero2 = INT_MAX;
  else
    numero2 = strtol(num2, NULL, 10);

  if(numero1 >= -INT_MAX && numero1<= INT_MAX){
    if(numero2 >= -INT_MAX && numero2<= INT_MAX){
      if(numero1 <= numero2){
        AVLTree arbol = itree_crear();
        Intervalo* intervalo = malloc(sizeof(Intervalo));
        intervalo->inicio = numero1;
        intervalo->final = numero2;
        arbol = itree_insertar(arbol, intervalo);
        hash_insertar(hash, alias, arbol);
      }
      else {
        printf("El inicio del intervalo es menor que el final\n");
      }
    }
    else{
      printf("Valores fuera del rango de los enteros\n");
    }
  } else {
    printf("Valores fuera del rango de los enteros\n");
  }
  free(resto);
}

int error_operacion(char end, char* resto){
  if((end != '\0' || strlen(resto) != 0) /* falta modificar para que funcione con complemento*/){
    printf("Error en la forma de la operacion a ejecutar\n");
    return 0;
  }
  return 1;
}

void ejecutar_operacion(Hash* hash, char* alias, char* operacion){
  char* alias1 = calloc(100, sizeof(char));
  char* alias2 = calloc(100, sizeof(char));
  char* resto = calloc(50, sizeof(char));
  char end, op;
  sscanf(operacion, "%99s %c %99s %49[^\n]%c", alias1, &op, alias2, resto, &end);
  AVLTree final = NULL;
  if(op == '|' && error_operacion(end, resto)){
    AVLTree operando1 = hash_conjunto(hash, alias1);
    AVLTree operando2 = hash_conjunto(hash, alias2);
    if (operando1 != NULL && operando2 != NULL)
      final = conjuntoavl_union(hash_conjunto(hash, alias1), hash_conjunto(hash, alias2));
    }
  else if (op == '&' && error_operacion(end, resto)){
    AVLTree operando1 = hash_conjunto(hash, alias1);
    AVLTree operando2 = hash_conjunto(hash, alias2);
    if (operando1 != NULL && operando2 != NULL)
      final = conjuntoavl_interseccion(hash_conjunto(hash, alias1), hash_conjunto(hash, alias2));
  }
  else if (((op == '-') || (op == '-')) && error_operacion(end, resto)){
    AVLTree operando1 = hash_conjunto(hash, alias1);
    AVLTree operando2 = hash_conjunto(hash, alias2);
    if (operando1 != NULL && operando2 != NULL)
      final = conjuntoavl_resta(hash_conjunto(hash, alias1), hash_conjunto(hash, alias2));
  }
  else if (alias1[0] == '~' && op == '\0'){ 
    sscanf(alias1, "~%s", alias1);
    AVLTree operando1 = hash_conjunto(hash, alias1);
    if (operando1 != NULL)
      final = conjuntoavl_complemento(hash_conjunto(hash, alias1));
  }
  if(final == NULL)
    printf("No se logro realizar la operacion\n");
  else 
    hash_insertar(hash, alias, final);
  free(alias1);
  free(alias2);
  free(resto);
}


// Procesamiento de entrada.

void copiar_seccion(char *comando, char *parte, int i, int *cont,
                    int *indexToken, char eow) {
  if (comando[i] == eow) {
    (*indexToken)++;
    (*cont) = -1;
  } else
    parte[(*cont)] = comando[i];
}

char entrada_validar(char *comando, char* inicio, char* igual, char* operacion) {
  int i = 0, cont = 0;
  int indexToken = 0;
  char eows[] = "  ";

  for (; comando[i] != '\n' && comando[i] != '\r'; i++, cont++) {
    // Copiamos la opreracion
    if (indexToken == 2)
      copiar_seccion(comando, operacion, i, &cont, &indexToken, '\n');
    // Copiamos la igualdad
    if (indexToken == 1)
      copiar_seccion(comando, igual, i, &cont, &indexToken, eows[1]);
    // Copiamos el inicio.
    if (indexToken == 0)
      copiar_seccion(comando, inicio, i, &cont, &indexToken, eows[0]);
  }
  return funcion_verificar(inicio, igual, operacion, ++indexToken);
}

int main() {

  int salida = 1;
  Hash* hash = hash_crear(CAPACIDAD);

  printf("Interfaz 1.0\n");
  while (salida) {
    char *comando = malloc(sizeof(char) * LARGO);
    char *inicio = calloc(LARGO, sizeof(char));
    char *igual = calloc(LARGO, sizeof(char));
    char *operacion = calloc(LARGO, sizeof(char));
    int identificador;
    // leemos con \n incluido
    fgets(comando, LARGO, stdin);
    // Si se excede la capacidad maxima queda caracteres en el buffer,
    // en ese caso limpiamos el buffer y notificamos el error.
    if (strlen(comando) == LARGO - 1){
      scanf("%*[^\n]");
      scanf("%*c");
      identificador = EXCESO;
    } else {
      //comando[LARGO-1] = '\0';
      identificador = entrada_validar(comando, inicio, igual, operacion);
    }
    
    // Dependiendo del identificador la accion sera distinta.
    switch (identificador) {
    case OPERACION:
      //printf("Llegaste a la operacion\n");
      ejecutar_operacion(hash, inicio, operacion);
      break;
    
    case EXTENSION:
      //printf("Llegaste a extension\n");
      insertar_extension(hash, inicio, operacion);
      break;
    
    case COMPRENSION:
      //printf("Llegaste a comprension\n");
      insertar_comprension(hash, inicio, operacion);
      break;

    case EXCESO:
      printf("Largo excedido\n");
      break;

    case ERROR:
      printf("ERROR - Elemento invalido dentro del comando\n");
      break;
    
    case IMPRIMIR:
      //printf("Llegaste a imprimir\n");
      itree_imprimir(hash_conjunto(hash, igual), intervalo_imprimir);
      printf("\n");
      break;

    case SALIR:
      salida = 0;
      break;

    default:
      //printf("ERROR - Caso desconocido, no debiste llegar aqui...");
      break;
    }
    // Libero la memoria de los comandos
    free(comando);
    free(inicio);
    free(igual);
    free(operacion);
  }
  // Se destruye la tabla hash de conjuntos.
  hash_destruir(hash);
  return 0;
}

  /*
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
  */
  // TODO pasar las funciones de hash.c a hash.h