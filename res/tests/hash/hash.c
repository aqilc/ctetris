
//gcc hash.c hashfunc.c -o hash.exe

#include <stdio.h>
#include <string.h>
#include "hashfunc.h"
#include <stdlib.h>
#include <stdarg.h>

#include <windows.h>
int collisions = 0;
double get_time()
{
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);
    return (double)t.QuadPart/(double)f.QuadPart;
}

typedef struct hashitem {
  char* key;
  void* value;
  
  // Make the item a linked list in case of collisions
  struct hashitem* next;
} hashitem;
typedef struct hashtable {
  char** keys;
  hashitem** t;
  unsigned int size;
  unsigned int count;
} hashtable;

// Should make size about 1000
hashtable* ht(unsigned short size) {
  hashtable* h = malloc(sizeof(hashtable));
  h->size = size;
  h->count = 0;
  h->t = calloc(size, sizeof(hashitem*));
  return h;
}

// New <type> functions for creating objects on the heap on the fly
int* new_i(int i) { int* v = malloc(sizeof(int)); *v = i; return v; }
unsigned int* new_ui(unsigned int i) { unsigned int* v = malloc(sizeof(unsigned int)); *v = i; return v; }
float* new_f(float f) { float* v = malloc(sizeof(float)); *v = f; return v; }
char* new_c(char c) { char* s = malloc(2); s[0] = c; s[1] = NULL; return s; }


// Insert into table ht
hashtable* hti(hashtable* ht, char* key, void* data) {
  
  // Does the hash
  unsigned int place = hash(key, strlen(key)) % ht->size;
  
  // Since we have to account for collisions, store the ptr to the ptr so we can change the pointer it points to in case of an actual collision
  hashitem** cur = ht->t + place;
  while(*cur != NULL) {
    collisions ++;
    if (!strcmp(key, (*cur)->key)) { (*cur)->value = data; return ht; }
    cur = &((*cur)->next);
  }
  hashitem* i = malloc(sizeof(hashitem));
  i->key = key;
  i->value = data;
  i->next = NULL;
  *cur = i;
  
  return ht;
}

// Get value from ht with key
void* htg(hashtable* h, char* key) {
  unsigned int place = hash(key, strlen(key)) % h->size;
  hashitem* cur = h->t[place];
  if(!cur) return NULL;
  while(cur->next != NULL)
    if(strcmp(key, cur->key))
      cur = cur->next;
    else break;
  return cur->value;
}

void main(void) {
  hashtable* yes = ht(200);
  hti(yes, "lmao", "bruh");
  
  double t = get_time();
  for(int i = 0; i < yes->size; i ++){
    char* str = malloc(4);
    str[3] = '\0';
    sprintf(str, "%d", i);
    hti(yes, str, new_i(i));
  }
  printf("%f ms for insert\n", (get_time() - t) * 1000);
  
  t = get_time();
  for(int i = 0; i < yes->size; i ++){
    char* str = malloc(4);
    sprintf(str, "%d", i);
    htg(yes, str);
  }
  printf("\n%f ms for getting\n", (get_time() - t) * 1000);
  
  printf("%s\n", htg(yes, "lmao"));
  printf("%d collisions", collisions);
}
