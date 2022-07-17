#include <string.h>
#include <stdlib.h>
#include "hash.h"

// Creates a table of specified size
hashtable* ht(unsigned short size) {
  hashtable* h = malloc(sizeof(hashtable));
  h->size = size;
  h->count = 0;
  h->t = calloc(size, sizeof(hashitem*));
  return h;
}

// New<type> functions for creating objects on the heap on the fly
int* new_i(int i) { int* v = malloc(sizeof(int)); *v = i; return v; }
unsigned int* new_ui(unsigned int i) { unsigned int* v = malloc(sizeof(unsigned int)); *v = i; return v; }
float* new_f(float f) { float* v = malloc(sizeof(float)); *v = f; return v; }
char* new_c(char c) { char* s = malloc(2); s[0] = c; s[1] = '\0'; return s; }
char* new_s(char* s) { char* t = malloc(strlen(s)); return strcpy(t, s); }


// Insert into table ht
hashtable* hti(hashtable* ht, char* key, void* data) {
  
  // Does the hash
  unsigned int place = hash(key, strlen(key)) % ht->size;
  
  // Since we have to account for collisions, store the ptr to the ptr so we can change the pointer it points to in case of an actual collision
  hashitem** cur = ht->t + place;
  while(*cur != NULL) {
    if (!strcmp(key, (*cur)->key)) { (*cur)->value = data; return ht; }
    cur = &((*cur)->next);
  }
  
  // Makes a new item, and puts it into whatever cur was
  hashitem* i = malloc(sizeof(hashitem));
  i->key = key;
  i->value = data;
  i->next = NULL;
  *cur = i;
  
  return ht;
}

// Get value from ht with key
void* htg(hashtable* h, char* key) {
  
  // Gens hash
  unsigned int place = hash(key, strlen(key)) % h->size;
  
  // Checks the place where the hash resolves to, and returns null if there's nothing
  hashitem* cur = h->t[place];
  if(!cur) return NULL;
  
  // Until there's an exact match, loop through the linked list of keys and values
  while(cur->next != NULL)
    if(strcmp(key, cur->key))
      cur = cur->next;
    else break;
  
  // Return the resulting value
  return cur->value;
}

// Getter functions that return types so it's easier to deal with them
int htgi(hashtable* h, char* key) { return *(int*) htg(h, key); }
unsigned int htgui(hashtable* h, char* key) { return *(unsigned int*) htg(h, key); }
float htgf(hashtable* h, char* key) { return *(float*) htg(h, key); }
