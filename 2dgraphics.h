
#ifndef TWODGRAPHICS_H
#define TWODGRAPHICS_H

#include "util.h"
#include "glapi.h"

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct shapedata {
  vec2 pos;
  vec2 tex;
  vec4 col;
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
  bool enlarged;
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
  char* name;
  hashtable* chars;
  
  // Opengl stuffs
  unsigned char* tex;
  vec texsize;
  unsigned int gl;
  GLuint slot;
} charstore;
charstore* loadchars(FT_Library ft, FT_Face face, char* chars);

typedef enum{
  QUAD,
  TEXT
} drawprimitives;


// Init for opengl
void glinitgraphics();

// Text things(took so longgggggggg)
void tfont(char* name);
void tsiz(unsigned short size);
void text(char* text, unsigned short x, unsigned short y);

// Draw related commands
void draw(void);
void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void rect(int x, int y, int w, int h);
void c(float c1, float c2, float c3, float c4);

// Advanced stuff
void skip(drawprimitives prim, unsigned short n);
static void shapeinsert(shapedata* buf, unsigned short* ib, unsigned short bs, unsigned short is);
void shape(shapedata* data, unsigned short* ib, unsigned short bs, unsigned short is, vec4 col);
void shapecolor(vec4 col, unsigned short verts);

#endif
