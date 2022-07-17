
#include "shapes.h"
#include <linmath.h>

typedef struct shapedata {
  vec2 pos;
  vec4 col;
  float rounding;
  float aspect;
} shapedata;

shapedata* buf; // Vertex Buffer
unsigned short* ib; // Index Buffer we append to
size_t bsize = 0;
size_t isize = 0;
size_t bcur = 0; // Cursor in the buffer/size of the current buffer ig
size_t icur = 0;
bool rea = false; // Whether to re-allocate vertex buffers
vec4 color;

// Assumes you init the shapes context
void shapesdraw() {
  
}

static void shapesalloc(size_t sv, size_t si) {
  if(!buf) { buf = malloc(sv * sizeof(shapedata)); bsize = sv; }
  else if(bsize < bcur) {
    buf = realloc(buf, (bsize + sv) * sizeof(shapedata));
    bsize += sv; rea = true;
  }
  if(!ib) { ib = malloc(si * sizeof(unsigned short)); isize = si; }
  else if(isize < bcur) {
    ib = realloc(ib, (isize + si) * sizeof(unsigned short));
    isize += si; rea = true;
  }
}

void rect(int x, int y, int w, int h) {
  
}

void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  shapesalloc(4, 6);
  buf[bcur + 0].pos.x = (float) x1;
  buf[bcur + 1].pos.x = (float) x2;
  buf[bcur + 2].pos.x = (float) x3;
  buf[bcur + 3].pos.x = (float) x4;
  buf[bcur + 0].pos.y = (float) y1;
  buf[bcur + 1].pos.y = (float) y2;
  buf[bcur + 2].pos.y = (float) y3;
  buf[bcur + 3].pos.y = (float) y4;
  ib[icur + 0] = bcur + 0;
  ib[icur + 1] = bcur + 1;
  ib[icur + 2] = bcur + 2;
  ib[icur + 3] = bcur + 1;
  ib[icur + 4] = bcur + 3;
  ib[icur + 5] = bcur + 2;
  bcur += 4; icur += 6;
}
