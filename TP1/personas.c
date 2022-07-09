#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <time.h>
#include "cabeceras/personas.h"

Datos* parser(char* file) {
  int total_size = 200;
  // Reservo memoria para la estructura Datos y su array correspondiente.
  Datos* lista = (Datos*)malloc(sizeof(Datos));
  char** array = (char**)malloc(sizeof(char*) * total_size);
  // Abro el archivo de entrada
  FILE* fp;
  fp = fopen(file, "r");
  int linea;
  // Recorro hasta llegar al EOF
  for (linea = 0; !feof(fp); linea++) {
    // Reservo memoria para la linea a leer y leo del archivo
    char* buff = malloc(sizeof(char) * 64);
    if (fscanf(fp, "%63[^\n]\n", buff) != EOF) {
      // Si no hay mas char* donde guardar la linea, aumento el tamaño 
      if (total_size == linea) {
        total_size *= 2;
        array = (char**)realloc(array, sizeof(char*) * total_size);
      }
      // Agrego el terminador
      buff[strlen(buff)] = '\0';
      array[linea] = buff;
    }
  }
  fclose(fp);
  // Guardo los valores y retorno la estructura
  lista->array = array;
  lista->largo = linea;
  return lista;
}

void escritura(FILE* fp, char* first, int second, char* third) {
  fprintf(fp, "%s, %d, %s\n", first, second, third);
}

void escSalida(Datos* datosNac, Datos* datosPer, char* file, long cant) {
  srand(time(NULL));
  FILE* fp;
  fp = fopen(file, "w");
  int i;
  // Genero valores random para elegir persona, edad y nacimiento
  for (i = 0; i < cant; i++) {
    int ranPer = (rand() * rand()) % datosPer->largo;
    int ranEdad = (rand() % 100) + 1;
    int ranNac = rand() % datosNac->largo;
    // Escribo la informacion final de la persona
    escritura(fp, datosPer->array[ranPer], ranEdad, datosNac->array[ranNac]);
  }
  fclose(fp);
}

void freeDatos(Datos* array) {
  // Recorro el array de datos y libero la memoria
  int i;
  for (i = 0; i < array->largo; i++)
    free(array->array[i]);
  free(array->array);
  free(array);
}

int main(int argc, char* argv[]) {
  // Parseo los datos de entrada
  Datos* datosPer = parser(argv[1]);
  Datos* datosNac = parser(argv[2]);
  // Transformo en long la cantidad de personas pasada como argumento
  long cant = strtol(argv[4], NULL, 10);
  // Escribo el archivo de salida y libero la memoria
  escSalida(datosNac, datosPer, argv[3], cant);
  freeDatos(datosNac);
  freeDatos(datosPer);
  return 0;
}
