#ifndef DATOPERSONA_H_INLCUDED
#define DATOPERSONA_H_INLCUDED

// Estructura que continene los datos de la persona.
 typedef struct _Persona Persona;

// dlist_leer_crear lee el archivo de entrada con la informacion de las
// personas y genera la DList correspondiente. Es el nexo entre ambas.
DList dlist_leer_crear (const char *);

// Funciones del tipo visitantes utilizadas para interactuar con la dlist.

// persona_destruir libera la memoria pedida especificamente para la
// estructura Persona.
void persona_destruir(Persona*, void *);
// Imprime los datos de la persona en el archivo dado.
void persona_imprimir(Persona*, FILE *);
// contar_largoAumenta el entero pasado como argumento, lo cual puede ser
// utilizado para obtener el largo de la lista que contiene a la estructura.
void contar_largo(Persona*, int *);
// persona_crear devulve una estructura Persona con los datos indicados.
Persona* persona_crear(char*, int, char*);

// Funciones de comparacion entre personas.

// Método de comparación para ordenar las personas según su edad
int comparacion_edad(Persona*, Persona*);
// Método de comparación para ordenar las personas según su nombre
int comparacion_nombre(Persona*, Persona*);

#endif
