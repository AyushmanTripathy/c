#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Bucket_tag {
  char * key;
  int data;
  struct Bucket_tag * next;
} Bucket;

typedef struct {
  int size;
  int r;
  Bucket ** buckets;
} Hashmap;

int hashFunc(char * str, int size, int r) {
  int hash = 0;
  for (int i = 0; str[i] != '\0'; i++)
    hash = ((r * hash) + (int) str[i]) % size;
  return hash;
}

Hashmap * initHashmap(int size, int r) {
  Hashmap * map = (Hashmap *) malloc(sizeof(Hashmap));
  map->buckets = (Bucket **) malloc(size * sizeof(Bucket *));
  map->size = size;
  map->r = r;
  return map;
}

void addToHashmap(Hashmap * map, char * key, int value) {
  int hashKey = hashFunc(key, map->size, map->r);
  Bucket * b = (Bucket *) malloc(sizeof(Bucket));
  b->next = NULL;
  b->key = key;
  b->data = value;

  Bucket ** buckets = map->buckets;
  if (buckets[hashKey] == NULL) buckets[hashKey] = b;
  else {
    Bucket * iterator = buckets[hashKey];
    while(iterator->next != NULL) iterator = iterator->next;
    iterator->next = b;
  }
}

int getFromHashmap(Hashmap * map, char * key) {
  int hashKey = hashFunc(key, map->size, map->r);
  if ((map->buckets)[hashKey] != NULL) {
    Bucket * iterator = (map->buckets)[hashKey];
    while(strcmp(iterator->key, key) != 0) {
      iterator = iterator->next;
      if (iterator == NULL) {
        printf("key not found %s\n", key); 
      }
    }
    return iterator->data;
  } else printf("key not found\n"); 
}

int main() {
  Hashmap * map = initHashmap(13, 3);
  addToHashmap(map, "ayush", 12);
  printf("%d\n", getFromHashmap(map, "ayush"));
}
