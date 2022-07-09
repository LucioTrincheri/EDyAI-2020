#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "cabeceras/dlist.h"
#include "cabeceras/datopersona.h"



typedef DList(*Sort) (DList, Comparacion);


double crear_salida(DList lista, Sort fSort, Comparacion fCom, char * infAl, FILE * fp) {
  // Variables usadas para calcular el timpo de ejecucion.
  clock_t start_t, end_t;
  // Creacion de la lista.
  // Se establece el momento de inicio de ejecucion del algoritmo.
  start_t = clock();
  // Ejecucion del mismo.
  lista = fSort(lista, fCom);
  // Se establece el momento de fin de ejecucion del algoritmo.
  end_t = clock();
  // Se calcula el tiempo de ejecucion.
  double total = (double) (end_t - start_t) / CLOCKS_PER_SEC;

  // Impirmimos el contenido de la lista.
  fprintf(fp, "\nLista resultante de aplicar el algoritmo %s:\n\n", infAl);
  dlist_recorrer(lista, (Visitante) persona_imprimir, fp);

  return total;
}



int main(int argc, char* argv[]) {
  // argv 1 archivo entrada
  // argv 2 archivo salida1

  // Cargamos la informacion en la lista, la lista posee la
  // informacion en orden inverso a la del archivo

  DList lista = dlist_leer_crear(argv[1]);

  double tiempos[6];
  // Abrimos archivo.
  FILE *fp = fopen(argv[2], "w");


  // Al duplicar se queda correctamente.
  DList nueva = dlist_duplicar_rotar(lista);
  tiempos[0] = crear_salida(nueva, dlist_selectionSort, (Comparacion) comparacion_edad, "selection con la comparacion por edad", fp);
  dlist_destruir(nueva, NULL);

  nueva = dlist_duplicar_rotar(lista);
  tiempos[1] = crear_salida(nueva, dlist_selectionSort, (Comparacion) comparacion_nombre, "selection con la comparacion por nombre", fp);
  dlist_destruir(nueva, NULL);

  nueva = dlist_duplicar_rotar(lista);
  tiempos[2] = crear_salida(nueva, dlist_insertionSort, (Comparacion) comparacion_edad, "insertion con la comparacion por edad", fp);
  dlist_destruir(nueva, NULL);

  nueva = dlist_duplicar_rotar(lista);
  tiempos[3] = crear_salida(nueva, dlist_insertionSort, (Comparacion) comparacion_nombre, "insertion con la comparacion por nombre", fp);
  dlist_destruir(nueva, NULL);

  nueva = dlist_duplicar_rotar(lista);
  tiempos[4] = crear_salida(nueva, dlist_mergeSort, (Comparacion) comparacion_edad, "merge con la comparacion por edad", fp);
  dlist_destruir(nueva, NULL);

  nueva = dlist_duplicar_rotar(lista);
  tiempos[5] = crear_salida(nueva, dlist_mergeSort, (Comparacion) comparacion_nombre, "merge con la comparacion por nombre", fp);
  dlist_destruir(nueva, NULL);


  // Se cuenta el largo de la lista (tal vez innecesario).
  int *largoL = calloc(1, sizeof(int));
  dlist_recorrer(lista, (Visitante) contar_largo, largoL);
  fprintf(fp, "\nLa lista tenia %d elementos\n\nResumen:", *largoL);
  free(largoL);
  fprintf(fp,"\nAlgoritmo | por edad | por nombre\n Selecion   %fs   %fs\n", tiempos[0], tiempos[1]);
  fprintf(fp," Insertion  %fs   %fs\n", tiempos[0], tiempos[1]);
  fprintf(fp," Merge      %fs   %fs", tiempos[0], tiempos[1]);

  fclose(fp);
  dlist_destruir(lista, (Visitante) persona_destruir);

  return 0;
}
