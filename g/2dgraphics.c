
/*
  TODO:
  - Need to completely stop uploading draw buffers every frame
  - 
*/

#include "2dgraphics.h"

// temporary stuff
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb/stb_image_write.h"

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

static void shapeinsert(shapedata* buf, unsigned short* ib, unsigned short bs, unsigned short is);
static void shape(shapedata* data, unsigned short* ib, unsigned short bs, unsigned short is, vec4 col);

// Data for the texture atlas for fonts and shapes
#define TEXTUREW 512
#define TEXTUREH 512
static unsigned short textsize = 48;

// Data for drawing shapes
static shapeheap* sh = NULL;
static drawcontext* ctx = NULL;
static char textures = 0;

// Current font and font store
static hashtable* cses = NULL;
static char* font = NULL;

// Context color
static vec4 col = { 1.0f, 1.0f, 1.0f, 1.0f };

// Scenes api, to save as many draws and buffer swaps as possible :D
// static int scene;

// Initializes everything for text, including setting up textures etc
void glinitgraphics() {
  initcontext(&ctx, "d:/projects/c/ctetris/res/shaders/graphics.glsl");

  // Sets up layout
  ctx->layout = calloc(sizeof(vlayout), 1);
  lpushf(ctx->layout, 2);
  lpushf(ctx->layout, 2);
  lpushf(ctx->layout, 4);
  
  // Allocs the heap
  sh = calloc(1, sizeof(shapeheap));
  sh->data.b = malloc(10 * sizeof(shapedata));
  sh->data.size = 10;
  sh->data.cur = 0;
  sh->ib.b = malloc(10 * sizeof(shapedata));
  sh->ib.size = 10;
  sh->ib.cur = 0;
  sh->enlarged = true;

  cses = ht(10);
}

void tfont(char* name) {
  charstore* cs = htg(cses, name);
  if(!cs->gl) {

    // Sets active texture to the next one in the array
    activet(textures);

    // Gets textureid and stuff
    cs->gl = texture(cs->tex, cs->texsize.w, cs->texsize.h, GL_RED);
    cs->slot = textures;

    // Sets the texture sampler to the one we just made
    setui("u_tex", textures);
    textures ++;
    return;
  }

  activet(cs->slot);
  bindt(cs->gl);
  setui("u_tex", cs->slot);
}

void tsiz(unsigned short size) { textsize = size; }
void text(char* text, unsigned short x, unsigned short y) {
  float scale = (float) textsize / 48.0f;
  
  // Sets up context
  context(ctx);

  // The charstore we're using to get the characters
  charstore* cs;

  // If there's no fonts loaded into the font store, don't draw anything
  if(!cses->count) return;
  else if(!font) {
    font = ((charstore*) htg(cses, cses->keys[0]))->name;
    tfont(font);
    cs = htg(cses, font);
  } else cs = htg(cses, font);
  
  // Sets up buffers
  unsigned int len = strlen(text);
  shapedata* vb = malloc(4 * len * sizeof(shapedata)); // 4 vertices per letter for the rectangle that the letter has
  unsigned short* ib = (unsigned short*) malloc(6 * sizeof(unsigned short) * len); // 6 vertices(2 triangles) per char
  
  // Generates every vertex
  unsigned short cur = sh->data.cur;
  float xp, yp, w, h, tx, ty, th, tw;
  vec4 col = { 1.0, 1.0, 1.0, 1.0 };
  unsigned int i = 0;
  for(; i < len; text++, i++) {

    // Gets the Char object from the hashmap for the character lib
    char* tmp = new_c(*text);
    Char* c = htg(cs->chars, tmp);
    free(tmp);
    
    if(!c) printf("bruh wtf u drawin \"%c\"", *text);
    
    xp = (float) x + c->bearing.x * scale;
    yp = (float) y - c->bearing.y * scale;
    w = (float) c->size.w * scale;
    h = (float) c->size.h * scale;

    tx = c->place.x / (float) cs->texsize.w;
    ty = c->place.y / (float) cs->texsize.h;
    tw = c->size.w / (float) cs->texsize.w;
    th = c->size.h / (float) cs->texsize.h;
    
    vb[i * 4] = (shapedata) {{ xp, yp }, { tx, ty }, { col[0], col[1], col[2], col[3] }};
    vb[i * 4 + 1] = (shapedata) {{ xp + w, yp }, { tx + tw, ty }, { col[0], col[1], col[2], col[3] }};
    vb[i * 4 + 2] = (shapedata) {{ xp, yp + h }, { tx, ty + th }, { col[0], col[1], col[2], col[3] }};
    vb[i * 4 + 3] = (shapedata) {{ xp + w, yp + h }, { tx + tw, ty + th }, { col[0], col[1], col[2], col[3] }};

    // Advance cursors for next glyph
    x += c->advance * scale;
  }
  
  // Sets all of the indexes of the index buffer, pretty straightforward
  for(i = 0; i < len; i ++) {
    ib[i * 6] = i * 4 + 3 + cur;
    ib[i * 6 + 1] = i * 4 + 0 + cur;
    ib[i * 6 + 2] = i * 4 + 2 + cur;
    
    ib[i * 6 + 3] = i * 4 + 1 + cur;
    ib[i * 6 + 4] = i * 4 + 3 + cur;
    ib[i * 6 + 5] = i * 4 + 0 + cur;
  }

  shapeinsert(vb, ib, 4 * len, 6 * len);
  free(vb);
  free(ib);
}

void draw() {
  if(!ctx->vbid) {
    ctx->vbid = create_vb(sh->data.b, sh->data.cur * sizeof(shapedata));
    ctx->ibid = create_ib(sh->ib.b, sh->ib.cur * sizeof(unsigned short));
    lapply(ctx->layout);
  } else if(sh->enlarged) {
    // bindv(ctx->vbid);
    glBufferData(GL_ARRAY_BUFFER, sh->data.cur * sizeof(shapedata), sh->data.b, GL_DYNAMIC_DRAW);
    // bindi(ctx->ibid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sh->ib.cur * sizeof(unsigned short), sh->ib.b, GL_DYNAMIC_DRAW);
  } else if(sh->changed) {
    // bindv(ctx->vbid);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sh->data.cur * sizeof(shapedata), sh->data.b);
    // bindi(ctx->ibid);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sh->ib.cur * sizeof(unsigned short), sh->ib.b);
  }

  glDrawElements(GL_TRIANGLES, sh->ib.cur, GL_UNSIGNED_SHORT, NULL);//sh->ib.b);

  // Resets cursors and since the data has been processed, reset the changed and enlarged booleans
  sh->data.cur = 0;
  sh->ib.cur = 0;
  sh->changed = false;
  sh->enlarged = false;
}

charstore* loadchars(FT_Library ft, FT_Face face, char* chars) {
  
  // Creates a new charstore
  charstore* pog = calloc(sizeof(charstore), 1);
  pog->chars = ht(200);
  vec texsize = { TEXTUREW, TEXTUREH };
  pog->texsize = texsize;
  pog->tex = malloc(pog->texsize.x * pog->texsize.y * sizeof(unsigned char));
  pog->name = new_s(face->family_name);

  // Root node for the texture fitting algo
  CharNode root = {.s = texsize};

  int len = strlen(chars);
  for(int i = 0; i < len; i ++) {
    if(FT_Load_Char(face, chars[i], FT_LOAD_RENDER)) printf("Failed to load glyph '%c'\n", chars[i]);
    
    int width = face->glyph->bitmap.width;
    int height = face->glyph->bitmap.rows;
    vec size = { width, height };
      
    CharNode* rec = charnode_insert(&root, &size);
    if(!rec) { printf("Could not fit char %c into the texture map\n", chars[i]); break; }
    
    // Shorthands
    int x = rec->p.x;
    int y = rec->p.y;
    int w = rec->s.w;
    int h = rec->s.h;
    
    // Defines the metrics for the glyph
    Char* ch = malloc(sizeof(Char));
    ch->c = chars[i];
    ch->size = size;
    ch->bearing = (vec) { face->glyph->bitmap_left, face->glyph->bitmap_top };
    ch->advance = face->glyph->advance.x >> 6; // bitshift by 6 to get value in pixels (2^6 = 64)

    // Remembers the place of the glyph in the atlas
    ch->place.x = x;
    ch->place.y = y;
    
    // big boi for loop that writes the bitmap buffer from the freetype glyph into the font atlas
    for(int i = 0; i < w * h; i ++) pog->tex[x + y * texsize.w + i % w + (i / w * texsize.h)] = face->glyph->bitmap.buffer[i];
    
    // Inserts it into the map so we can look it up later
    hti(pog->chars, new_c(chars[i]), ch);
  }

  // Frees up a crap ton of memory
  charnode_free(root.child[0]);
  charnode_free(root.child[1]);

  // Draws a little white square in the corner of the texture atlas for shapes
  for(int i = 16; i > 0; i --) pog->tex[texsize.w * texsize.h - i % 4 - (i / 4 * texsize.h)] = 255;

  // So we can ~~admire~~inspect the generated font atlas later
  // stbi_write_png("bitmap.png", texsize.w, texsize.w, 1, pog->tex, texsize.w);

  // Sets it as the current font
  hti(cses, new_s(pog->name), pog);
  tfont(pog->name);

  return pog;
}

void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

  // Creates the shape
  shapedata buf[4];
  buf[0].pos[0] = (float) x1;
  buf[1].pos[0] = (float) x2;
  buf[2].pos[0] = (float) x3;
  buf[3].pos[0] = (float) x4;
  buf[0].pos[1] = (float) y1;
  buf[1].pos[1] = (float) y2;
  buf[2].pos[1] = (float) y3;
  buf[3].pos[1] = (float) y4;

  // Creates the index buffer
  unsigned short ib[6];
  ib[0] = 0;
  ib[1] = 1;
  ib[2] = 2;
  ib[3] = 1;
  ib[4] = 2;
  ib[5] = 3;

  shape(buf, ib, 4, 6, col);
  shapeinsert(buf, ib, 4, 6);
}


// Create a temporary array of texture coords so we can properly set them later
static float bruhwhyc[] = { 1.0f - 2.5f/ (float)TEXTUREW, 1.0f - 2.5f/ (float) TEXTUREH, 1.0f - 2.5f/ (float) TEXTUREW, 1.0f, 1.0f, 1.0f - 2.5f/ (float) TEXTUREH, 1.0f, 1.0f };
static void shape(shapedata* data, unsigned short* ib, unsigned short bs, unsigned short is, vec4 col) {
  int cur = sh->data.cur;
  for(int i = 0; i < is; i ++) ib[i] += cur;

  // Sets up color and texture coords for all 4 verts at once
  for(char i = 0; i < bs; i ++)
    memcpy(data[i].col, col, sizeof(vec4)), memcpy(data[i].tex, bruhwhyc + (i%4) * 2, sizeof(vec2));
}

void shapecolor(vec4 col, unsigned short verts) {
  for(unsigned short i = verts; i > 0; i --)
    memcpy(sh->data.b[sh->data.cur - i].col, col, sizeof(vec4));
}

void rect(int x, int y, int w, int h) {
  quad(x, y, x + w, y, x, y + h, x + w, y + h);
}

void fill(float c1, float c2, float c3, float c4) {
  col[0] = c1; col[1] = c2; col[2] = c3; col[3] = c4; }

void skip(drawprimitives prim, unsigned short n) {
  sh->data.cur += n * 4;
  sh->ib.cur += n * 6;
}
static void shapeinsert(shapedata* buf, unsigned short* ib, unsigned short bs, unsigned short is) {

  // If the memory buffer for the buffer data heap is not enough,
  if(sh->data.size < sh->data.cur + bs){

    // Ask for bigger size
    sh->data.b = realloc(sh->data.b, sizeof(shapedata) * (sh->data.cur + bs));

    // Copy into it
    memcpy(sh->data.b + sh->data.cur, buf, bs * sizeof(shapedata));

    // Advance cursor and size
    sh->data.cur += bs;
    sh->data.size = sh->data.cur;

    // Set changed and enlarged so we can reupload the data later
    sh->changed = true;
    sh->enlarged = true;
  }
  // If it is enough, but the memory isn't the same
  else// if (memcmp(sh->data.b + sh->data.cur, buf, bs))
  {
    // Copy the memory in and just change it
    memcpy(sh->data.b + sh->data.cur, buf, bs * sizeof(shapedata));
    sh->data.cur += bs;
    sh->changed = true;
  }// else sh->data.cur += bs;

  if(sh->ib.size < sh->ib.cur + is){
    sh->ib.b = realloc(sh->ib.b, sizeof(unsigned short) * (sh->ib.cur + is));
    memcpy(sh->ib.b + sh->ib.cur, ib, is * sizeof(unsigned short));
    sh->ib.cur += is;
    sh->ib.size = sh->ib.cur;
    sh->changed = true;
    sh->enlarged = true;
  }
  else// if (memcmp(sh->ib.b + sh->ib.cur, buf, is))
  {
    memcpy(sh->ib.b + sh->ib.cur, ib, is * sizeof(unsigned short));
    sh->changed = true;
    sh->ib.cur += is;
  }// else sh->ib.cur += is;
}