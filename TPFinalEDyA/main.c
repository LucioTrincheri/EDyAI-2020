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

// Esta operacion toma el alias y un conjunto que se sabe
// que es del tipo por extension.
// El conjunto de numeros es procesado uno por uno mediante strtok.
// Cada uno debe pasar por los requisitos necesarios para ser agregado
// como intervalo del conjunto final
// Si alguno de los valores numericos no cumple con los requisitos 
// (ej. que este fuera del rango de los enteros, que sea un numero de 
// coma flotante, etc) el comando se desecha y se borra el arbol generado 
void insertar_extension(Hash* hash, char* alias, char* conjunto){
  char* numeros = calloc(LARGO, sizeof(char));
  char* resto = calloc(LARGO, sizeof(char));
  sscanf(conjunto, "{%255[^}]} %255[^\n]", numeros, resto);
  const char ch = '.';
  if(strchr(numeros, ch) != NULL || strlen(resto) != 0){
    printf("Error del conjunto por extension\n");
    free(numeros);
    free(resto);
    return;
  }
  // Caso de ingresar el vacio
  AVLTree arbol = itree_crear();
  if(strlen(numeros) == 0) {
    Intervalo* vacio = malloc(sizeof(Intervalo));
    vacio->inicio = INVALINI;
    vacio->final = INVALFIN;
    arbol = itree_insertar(arbol, vacio);
    hash_insertar(hash, alias, arbol);
    free(numeros);
    free(resto);
    return;
  }
  // Separo el string de numeros en sus individuales valores
  // para poder ser procesados
  const char separador[2] = ",";
  char* token = strtok(numeros, separador);
  while(token != NULL){
    Intervalo* valor = malloc(sizeof(Intervalo));
    long numero = strtol(token, NULL, 10);
    if(numero >= -INT_MAX && numero<= INT_MAX && strcmp(token, "-INF") != 0 && strcmp(token, "INF") != 0){
      valor->inicio = numero;
      valor->final = numero;
      arbol = itree_insertar_disyuncion(valor, arbol);
      free(valor);
    } else {
      itree_destruir(arbol);
      printf("Intervalo fuera del rango de los enteros\n");
      free(numeros);
      free(resto);
      return;
    }
    token = strtok(NULL, separador);
  }
  free(numeros);
  free(resto);
  hash_insertar(hash, alias, arbol);
}

// Esta operacion toma el alias y un conjunto que se sabe
// que es del tipo por comprension.
// Segun la informacion que contiene el conjunto, se ve si el mismo 
// puede ser aceptado al cumplir con los requisitos o debe ser desechado.
// En ese caso, se notifica al usuario de la razon por la cual pasa esto
void insertar_comprension(Hash* hash, char* alias, char* conjunto){
  char var1, var2, num1[20], num2[20];
  char* resto = calloc(LARGO, sizeof(char));
  const char ch = '.';
  if(strchr(conjunto, ch) != NULL){
    printf("Se encontraron numeros de coma flotante\n");
    free(resto);
    return;
  }
  sscanf(conjunto, "{%c : %19s <= %c <= %19[^}\n]} %255[^\n]", &var1, num1, &var2, num2, resto);
  if(strlen(resto) != 0 || var1 != var2 || var1 != 'x'){
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
  // Compruebo si cumple con los requisitos para ser agregado
  if(numero1 >= -INT_MAX && numero1<= INT_MAX){
    if(numero2 >= -INT_MAX && numero2<= INT_MAX){
      AVLTree arbol = itree_crear();
      Intervalo* intervalo = malloc(sizeof(Intervalo));
      if(numero1 <= numero2){
        intervalo->inicio = numero1;
        intervalo->final = numero2;
      }
      else { // Caso intervalo vacio
        intervalo->inicio = INVALINI;
        intervalo->final = INVALFIN;
      }
      arbol = itree_insertar(arbol, intervalo);
      hash_insertar(hash, alias, arbol);
    }
    else{
      printf("Valores fuera del rango de los enteros\n");
    }
  } else {
    printf("Valores fuera del rango de los enteros\n");
  }
  free(resto);
}

// Operacion que retorna si la cadena de error "resto" tiene caracteres
// Si esto es positivo, el comando debe ser desechado ya que contiene errores

int error_operacion(char* resto){
  if(strlen(resto) != 0){
    printf("Error en la forma de la operacion a ejecutar\n");
    return 0;
  }
  return 1;
}

// Esta operacion toma el alias y la operacion ya separadas.
// Segun la informacion que contiene la cadena operacion, realiza la 
// operacion correspondiente, guardando en la tabla hash el resultado
// si fue satisfactoria. En caso contrario notifica al usuario
void ejecutar_operacion(Hash* hash, char* alias, char* operacion){
  char* alias1 = calloc(100, sizeof(char));
  char* alias2 = calloc(100, sizeof(char));
  char* resto = calloc(50, sizeof(char));
  char op = 0;
  sscanf(operacion, "%99s %c %99s %49[^\n]", alias1, &op, alias2, resto);
  AVLTree final = NULL;
  if(op == '|' && error_operacion(resto)){
    AVLTree operando1 = hash_conjunto(hash, alias1);
    AVLTree operando2 = hash_conjunto(hash, alias2);
    if (operando1 != NULL && operando2 != NULL)
      final = conjuntoavl_union(hash_conjunto(hash, alias1), hash_conjunto(hash, alias2));
    }
  else if (op == '&' && error_operacion(resto)){
    AVLTree operando1 = hash_conjunto(hash, alias1);
    AVLTree operando2 = hash_conjunto(hash, alias2);
    if (operando1 != NULL && operando2 != NULL)
      final = conjuntoavl_interseccion(hash_conjunto(hash, alias1), hash_conjunto(hash, alias2));
  }
  else if (((op == '-') || (op == '-')) && error_operacion(resto)){
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

// Una vez separada la cadena en sus parte respectivas, decide que curso seguir
// con la entrada segun como estan compuestas las mismas
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
        if(strncmp(final, "{x :", 4) == 0)
          return COMPRENSION;
        else
          return EXTENSION;
      else
        return OPERACION;
    }
  }
  return ERROR;
}

// Funcion de le entrega del TP2 (sin modificar)
// Copia partes del comando hasta ciertos caracteres de fin de palabra 
void copiar_seccion(char *comando, char *parte, int i, int *cont,
                    int *indexToken, char eow) {
  if (comando[i] == eow) {
    (*indexToken)++;
    (*cont) = -1;
  } else
    parte[(*cont)] = comando[i];
}

// Funcion encargada de separar el comando de entrada original en sus partes
// respectivas. Una vez hecho esto, segun el contenido de las mismas retorna
// el resultado de funcion_verificar
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
  // Inicializo valores de salida y la tabla hash
  int salida = 1;
  Hash* hash = hash_crear(CAPACIDAD);

  printf("Interfaz 1.0\n");
  // Mientras que no se ingrese salir por consola, el programa
  // seguira pidiendo la entrada de cadenas para operar sobre ellas
  while (salida) {
    // Reservo la memoria para el comando y su separacion en partes
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
      identificador = entrada_validar(comando, inicio, igual, operacion);
    }
    
    // Dependiendo del identificador la accion sera distinta.
    switch (identificador) {
    case OPERACION:
      ejecutar_operacion(hash, inicio, operacion);
      break;
    
    case EXTENSION:
      insertar_extension(hash, inicio, operacion);
      break;
    
    case COMPRENSION:
      insertar_comprension(hash, inicio, operacion);
      break;

    case EXCESO:
      printf("Largo excedido\n");
      break;

    case ERROR:
      printf("ERROR - Elemento invalido dentro del comando\n");
      break;
    
    case IMPRIMIR:
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