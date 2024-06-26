#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  if (map == NULL || key == NULL) return; 
  
  int posicion = hash(key,map->capacity);
  Pair* aux = malloc(sizeof(Pair));
  int start = posicion; 
  
  while (map->buckets[posicion] != NULL) {
    if (is_equal(map->buckets[posicion]->key, key)) {
      free(map->buckets[posicion]->value);
      map->buckets[posicion]->value = value;
      return;
    }
    posicion = (posicion + 1) % map->capacity; 
    if (posicion == start) 
        return;
  }
  aux = createPair(key, value);
  map->buckets[posicion] = aux;
  map->size++;
}


void enlarge(HashMap * map) {
    int old_capacity = map->capacity;
    Pair **old_array = map->buckets;
  
    map->capacity = 2 * old_capacity;
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair*));
    map->size = 0;
  for(int i = 0; i < old_capacity; i++){
    if(old_array[i] != NULL){
      insertMap(map, old_array[i]->key, old_array[i]->value);
      free(old_array[i]);
    }
  }
  free(old_array); //no borrar (testing purposes)
  }
  




HashMap * createMap(long capacity) {
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Pair **)calloc(capacity, sizeof(Pair*));
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;
    return map;
}

void eraseMap(HashMap * map,  char * key) {
   if (map == NULL || key == NULL){
     return;
   } 
    
    int posicion = hash(key, map->capacity);
    while (map->buckets[posicion] != NULL &&
           map->buckets[posicion]->key != NULL) {
      if (is_equal(map->buckets[posicion]->key, key)) {
        map->buckets[posicion]->key = NULL;
        map->buckets[posicion]->value = NULL;
        map->size--;
        return;
      }
      posicion = (posicion + 1) % map->capacity;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
  if (map == NULL || key == NULL) return NULL;  //Verificación inicial de argumentos nulos

  int posicion = hash(key, map->capacity);
  int start = posicion;  // Guarda la posición inicial para evitar bucles infinitos

  // Bucle para manejar el sondeo lineal
  while (map->buckets[posicion] != NULL) {
      if (map->buckets[posicion]->key != NULL && is_equal(map->buckets[posicion]->key, key)) {
          map->current = posicion;  // Actualiza la posición actual si se encuentra la clave
          return map->buckets[posicion];
      }
      posicion = (posicion + 1) % map->capacity;  // Sigue buscando con sondeo lineal (Manejo de colisiones)
      if (posicion == start) break;  // Evita bucles infinitos verificando si ha recorrido toda la tabla
  }
  return NULL;  // Retorna NULL si no encuentra la clave
}

/*if(map == NULL || key == NULL) return NULL;
int posicion = hash(key, map->capacity);
while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL){
  if(is_equal(map->buckets[posicion]->key, key)){
    map->current = posicion;
    return map->buckets[posicion];
  }
}
  return NULL;*/

Pair * firstMap(HashMap * map) {
if(map == NULL|| map->size == 0)
    return NULL;
for(int i = 0; i < map->capacity; i++){
  if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
    map->current = i;
    return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  if(map == NULL || map->size == 0 || map->current == -1)
    return NULL;
  
  for(int i = map->current + 1; i < map->capacity; i++){
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    }
    
  }
  
  return NULL;
}
