
#include "graphics.h"

#define TEXTUREW 512
#define TEXTUREH 512
static unsigned short textsize = 48;
static shapeheap* sh = NULL;
static drawcontext* ctx = NULL;
static unsigned short textures = 0;
charstore** cses = NULL;

// Initializes everything for text, including setting up textures etc
void glinitgraphics() {
  initcontext(&ctx, "res/shaders/graphics.glsl");

  // Sets up layout
  ctx->layout = calloc(sizeof(vlayout), 1);
  lpushf(ctx->layout, 2);
  lpushf(ctx->layout, 2);
  lpushf(ctx->layout, 4);
  
  // Mallocs the heap
  sh = calloc(1, sizeof(shapeheap));
  sh->data.b = malloc(10 * sizeof(shapedata));
  sh->data.size = 10;
  sh->ib.b = malloc(10 * sizeof(shapedata));
  sh->ib.size = 10;
  sh->enlarged = true;
}

void tfont(charstore* cs) {
  if(!cs->gl) {

    // Sets active texture to the next one in the array
    activet(textures);

    // Gets textureid and stuff
    cs->gl = texture(cs->tex, cs->texsize.w, cs->texsize.h, GL_RED);
    cs->slot = textures;

    // Sets the texture sampler to the one we just made
    setui("u_tex", textures);
    textures ++;
  }
  else {
    activet(cs->slot);
    bindt(cs->gl);
    setui("u_tex", cs->slot);
  }
}

void tsiz(unsigned short size) { textsize = size; }
void text(charstore* cs, char* text, unsigned short x, unsigned short y) {

  float scale = (float) textsize / 48;

  
  // Sets up context
  context(ctx);
  if(!cs->gl) tfont(cs);
  bindt(cs->gl);
  
  // Sets up buffers
  unsigned int len = strlen(text);
  shapedata* vb = malloc(4 * len * sizeof(shapedata)); // starting vertices, and then 2 per character
  unsigned short* ib = (unsigned short*) malloc(6 * sizeof(unsigned short) * len); // 6 vertices(2 triangles) per char
  
  // Generates every vertex
  unsigned short cur = sh->data.cur;
  float xp, yp, w, h, tx, ty, th, tw;
  vec4 col = { 1.0, 1.0, 1.0, 1.0 };
  int i = 0;
  for(; i < len; text++, i++) {
    Char* c = htg(cs->chars, new_c(*text));
    if(!c) printf("bruh wtf u drawin \"%c\"", *text);
    
    xp = (float) x + c->bearing.x;
    yp = (float) y + c->bearing.y;
    w = (float) c->size.w * scale;
    h = (float) c->size.h * scale;

    tx = c->place.x / (float) cs->texsize.w;
    ty = c->place.y / (float) cs->texsize.h;
    tw = c->size.w / (float) cs->texsize.w;
    th = c->size.h / (float) cs->texsize.h;
    
    vb[i * 4] = (shapedata) {{ xp, yp }, { tx, ty + th }, { 1.0, 1.0, 1.0, 1.0 }};
    vb[i * 4 + 1] = (shapedata) {{ xp + w, yp }, { tx + tw, ty + th }, { 1.0, 1.0, 1.0, 1.0 }};
    vb[i * 4 + 2] = (shapedata) {{ xp, yp + h }, { tx, ty }, { 1.0, 1.0, 1.0, 1.0 }};
    vb[i * 4 + 3] = (shapedata) {{ xp + w, yp + h }, { tx + tw, ty }, { 1.0, 1.0, 1.0, 1.0 }};

    // Advance cursors for next glyph
    x += c->advance;
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
}

void draw() {
  if(!ctx->vbid) {
    ctx->vbid = create_vb(sh->data.b, 4 * sh->data.cur * sizeof(shapedata));
  } else if(sh->enlarged) {
    //bindv(g.contexts.text->vbid);
    glBufferData(GL_ARRAY_BUFFER, 4 * sh->data.cur * sizeof(shapedata), sh->data.b, GL_DYNAMIC_DRAW);
  lapply(ctx->layout);
  } else if(sh->changed) {
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sh->data.cur * sizeof(shapedata), sh->data.b);
  }

  glDrawElements(GL_TRIANGLES, sh->ib.cur, GL_UNSIGNED_SHORT, sh->ib.b);
  sh->data.cur = 0;
  sh->ib.cur = 0;
}

charstore* loadchars(FT_Library ft, FT_Face face, char* chars) {
  
  // Creates a new charstore
  charstore* pog = calloc(sizeof(charstore), 1);
  pog->chars = ht(200);
  vec texsize = { TEXTUREW, TEXTUREH };
  pog->texsize = texsize;
  pog->tex = malloc(pog->texsize.x * pog->texsize.y * sizeof(unsigned char));

  // Root node for the texture fitting algo
  CharNode root = { .s = texsize };
  
  int len = strlen(chars);
  for(int i = 0; i < len; i ++) {
    if(FT_Load_Char(face, chars[i], FT_LOAD_RENDER)) { printf("Failed to load glyph '%c'\n", i); }
    
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

  puts("bruh");

  // Draws a little white square in the corner of the texture atlas for shapes
  for(int i = 16; i > 0; i --) pog->tex[texsize.w * texsize.h - i % 4 - (i / 4 * texsize.h)] = 255;
  
  // So we can ~~admire~~inspect the generated font atlas later
  stbi_write_png("bitmap.png", texsize.w, texsize.w, 1, pog->tex, texsize.w);
  return pog;
}

void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, vec4 col) {

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
float bruhwhyc[] = { 1.0f - 3.0f/ (float)TEXTUREW, 1.0f - 3.0f/ (float) TEXTUREH, 1.0f - 3.0f/ (float) TEXTUREW, 1.0f, 1.0f, 1.0f - 3.0f/ (float) TEXTUREH, 1.0f, 1.0f };
void shape(shapedata* data, unsigned short* ib, unsigned short bs, unsigned short is, vec4 col) {
  int cur = sh->data.cur;
  for(int i = 0; i < is; i ++) ib[i] += cur;

  // Sets up color and texture coords for all 4 verts at once
  for(char i = 0; i < bs; i ++)
    memcpy(data[i].col, col, sizeof(vec4)), memcpy(data[i].tex, bruhwhyc + (i%4) * 2, sizeof(float) * 2);
}

static void shapeinsert(shapedata* buf, unsigned short* ib, size_t bs, size_t is) {
  if(sh->data.size < sh->data.cur + bs){
    puts("adding stuff to vb");
    sh->data.b = realloc(sh->data.b, sizeof(shapedata) * (sh->data.cur + bs));
    memcpy(sh->data.b + sh->data.cur, buf, bs * sizeof(shapedata));
    sh->data.cur += bs;
    sh->data.size = sh->data.cur;
    sh->changed = true;
    sh->enlarged = true;
  }
  else if (memcmp(sh->data.b + sh->data.cur, buf, bs)) {
    memcpy(sh->data.b + sh->data.cur, buf, bs);
    sh->changed = true;
  } else sh->data.cur += bs;

  if(sh->ib.size < sh->ib.cur + is){
    puts("adding stuff to ib");
    sh->ib.b = realloc(sh->ib.b, sizeof(unsigned short) * (sh->ib.cur + is));
    memcpy(sh->ib.b + sh->ib.cur, ib, is * sizeof(unsigned short));
    sh->ib.cur += is;
    sh->ib.size = sh->ib.cur;
    sh->changed = true;
    sh->enlarged = true;
  }
  else if (memcmp(sh->ib.b + sh->ib.cur, buf, is)) {
    memcpy(sh->ib.b + sh->ib.cur, ib, is * sizeof(unsigned short));
    sh->changed = true;
    sh->ib.cur += is;
  } else sh->ib.cur += is;
}
