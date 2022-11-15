
#include "hashfunc.h"

typedef struct hashitem {
  char* key;
  void* value;
  
  // Make the item a linked list in case of collisions
  struct hashitem* next;
} hashitem;

typedef struct hashtable {
  char** keys;
  hashitem** t;
  unsigned short size;
  unsigned int count;
  unsigned int keyssize;
} hashtable;

hashtable* ht(unsigned short size);
hashtable* hti(hashtable* ht, char* key, void* data);
void* htg(hashtable* h, char* key);

int* new_i(int i);
unsigned int* new_ui(unsigned int i);
float* new_f(float f);
char* new_c(char c);
char* new_s(char* s);

int htgi(hashtable* h, char* key);
unsigned int htgui(hashtable* h, char* key);
float htgf(hashtable* h, char* key);
