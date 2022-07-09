#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cabeceras/dlist.h"
#include "cabeceras/datopersona.h"

struct _Persona {
  char* nombre, * lugar;
  long edad;
};

Persona* persona_crear(char* nombre, int edadE, char* lugar) {
  Persona* nueva = malloc(sizeof(Persona));
  nueva->nombre = nombre;
  nueva->edad = edadE;
  nueva->lugar = lugar;
  return nueva;
}

void persona_destruir(Persona * persona, void * aux) {
  free(persona->nombre);
  free(persona->lugar);
  free(persona);
}

void contar_largo(Persona* persona, int * largo) {
  (*largo)++;
}

DList dlist_leer_crear(const char* filename) {
  DList lista = dlist_crear();
  char* edad = malloc(sizeof(char)*4);
  FILE* fp;
  fp = fopen(filename, "r");
  for (;!feof(fp);) {
  char* nombre = malloc(sizeof(char)*255);
  char* lugar = malloc(sizeof(char)*255);
  // Se lee la linea y almacena segun corresponde
  fscanf(fp, "%255[^,], %4[^,], %255[^\n]\n", nombre, edad, lugar);
  // Se transforma la edad de char a int para poder ser trabajada
  long edadE = strtol(edad, NULL, 10);
  // Se agrega el nodo a la lista

  lista = dnodo_agregar_inicio(lista, persona_crear(nombre, edadE, lugar));
  }
  fclose(fp);
  free(edad);
  return lista;
}

// persona_imprimir es del tipo Visitante
void persona_imprimir(Persona * persona, FILE * fp) {
  fprintf(fp, "%s, %ld, %s\n",persona->nombre, persona->edad, persona->lugar);
}

// comparacion_edad es del tipo Comparacion
int comparacion_edad(Persona* persona1, Persona* persona2) {
  return (persona1->edad <= persona2->edad);
}
// comparacion_nombre es del tipo Comparacion
int comparacion_nombre(Persona* persona1, Persona* persona2) {
  return (strcmp(persona1->nombre, persona2->nombre) <= 0);
}

