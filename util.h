#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include "util/hash.h"

typedef union vec {
  struct { int x; int y; };
  struct { int w; int h; };
} vec;
typedef struct pvec { double x, y; } pvec;


typedef struct CharNode {
  vec p; // Place
  vec s; // Size
  struct CharNode* child[2];
  bool filled;
} CharNode;
CharNode* charnode_insert(CharNode* pnode, vec* size);

typedef struct gamestate {
  double delta; // Delta time in the last frame
  float last;
  int width, height; // Starting width and height of window
} gamestate;


extern gamestate g;

char* read(char* file);
#endif


