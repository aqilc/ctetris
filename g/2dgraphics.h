
#ifndef TWODGRAPHICS_H
#define TWODGRAPHICS_H

// #include "util.h"
#include "glapi.h"
#include "../vec.h"
#include <linmath.h>

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct Char {
  // Place in texture
  u8 c;
  union vec place;
  union vec size;
  union vec bearing;
  u32 advance;
} Char;

typedef struct charstore {
  char* name;
  hashtable* chars;
  
  // Opengl stuffs
  u8* tex;
  union vec texsize;
  u32 gl;
  GLuint slot;
} charstore;
charstore* loadchars(FT_Library ft, FT_Face face, char* chars);

typedef struct imagedata {
  u32 id;
  GLuint slot;
  GLenum type;
  u16 uses;
  union vec size;
  bool charstore;
} imagedata;

typedef enum {
  QUAD,
  TEXT
} drawprimitives;



// Init for opengl
void glinitgraphics();

// Text things(took so longgggggggg)
void tfont(char* name);
void tsiz(u16 size);
void text(char* text, int x, int y);

// Draw related commands
void draw(void);
void rect(int x, int y, int w, int h);
void fill(float c1, float c2, float c3, float c4);

// Advanced stuff
void skip(drawprimitives prim, u16 n);
void shapecolor(vec4 col, u16 verts);

imagedata* loadimage(char* path);
void image(imagedata* image, int x, int y, int w, int h);

#endif

