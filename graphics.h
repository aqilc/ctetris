
#include "util.h"
#include "glapi.h"

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct shapedata {
  vec2 coords;
  vec2 tex;
  // vec3 col;
} shapedata;

typedef struct shapeheap {
  struct {
    shapedata* b;
    unsigned short cur;
    unsigned short size;
  } data;
  struct {
    unsigned short* b;
    unsigned short cur;
    unsigned short size;
  } ib;
  unsigned int shapes;
  bool changed;
} shapeheap;

typedef struct Char {
  // Place in texture
  char c;
  vec place;
  vec size;
  vec bearing;
  unsigned int advance;
} Char;

typedef struct charstore {
  hashtable* chars;
  size_t size;
  
  // Opengl stuffs
  unsigned char* tex;
  vec texsize;
  unsigned int gl;
  vlayout* layout;
} charstore;
charstore* loadgraphics(FT_Library ft, FT_Face face, char* chars);

void glinitgraphics(charstore* cs);
void tsiz(unsigned short size);
void text(charstore* cs, char* text, unsigned short x, unsigned short y);

void draw(void);
void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

static void shapeinsert(shapedata* buf, unsigned short* ib, size_t bs, size_t is);

