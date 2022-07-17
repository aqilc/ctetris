
#include "graphics.h"

unsigned short textsize = 48;
shapeheap* sh = NULL;

// Initializes everything for text, including setting up textures etc
void glinitgraphics(charstore* cs) {
  cs->gl = texture(cs->tex, cs->texsize.w, cs->texsize.h, GL_RED);
  cs->layout = calloc(sizeof(vlayout), 1);
  lpushf(cs->layout, 2);
  lpushf(cs->layout, 2);
  
  // Sets the texture to 0 just bc opengl gay
  setui("tex", 0);

  // Mallocs the heap
  sh = calloc(1, sizeof(shapeheap));
}

void tsiz(unsigned short size) { textsize = size; }
void text(charstore* cs, char* text, unsigned short x, unsigned short y) {
  float scale = (float) textsize / 48;
  
  // Sets up context
  context(g.contexts.text);
  if(!cs->gl) glinitgraphics(cs);
  bindt(cs->gl);
  
  // Sets up buffers
  unsigned int len = strlen(text);
  shapedata* vb = malloc(4 * len * sizeof(shapedata)); // starting vertices, and then 2 per character
  unsigned short* ib = malloc(6 * sizeof(unsigned short) * len); // 6 vertices(2 triangles) per char
  
  // Generates every vertex
  float xp, yp, w, h, tx, ty, th, tw;
  int i = 0;
  for(; *text; text++, i++) {
    Char* c = htg(cs->chars, new_c(*text));
    
    xp = (float) x + c->bearing.x;
    yp = (float) y + c->bearing.y;
    w = (float) c->size.w * scale;
    h = (float) c->size.h * scale;

    tx = c->place.x / (float) cs->texsize.w;
    ty = c->place.y / (float) cs->texsize.h;
    tw = c->size.w / (float) cs->texsize.w;
    th = c->size.h / (float) cs->texsize.h;
    
    vb[i * 4] = (shapedata) {{ xp, yp }, { tx, ty + th }};
    vb[i * 4 + 1] = (shapedata) {{ xp + w, yp }, { tx + tw, ty + th }};
    vb[i * 4 + 2] = (shapedata) {{ xp, yp + h }, { tx, ty }};
    vb[i * 4 + 3] = (shapedata) {{ xp + w, yp + h }, { tx + tw, ty }};
    
    // Advance cursors for next glyph
    x += c->advance;
  }
  
  // Sets all of the indexes of the index buffer, pretty straightforward
  for(i = 0; i < len; i ++) {
    ib[i * 6] = i * 4 + 3;
    ib[i * 6 + 1] = i * 4 + 0;
    ib[i * 6 + 2] = i * 4 + 2;
    
    ib[i * 6 + 3] = i * 4 + 1;
    ib[i * 6 + 4] = i * 4 + 3;
    ib[i * 6 + 5] = i * 4 + 0;
  }
  
  free(ib);
  free(vb);
}

void draw() {  
  if(!g.contexts.text->vbid) {
    g.contexts.text->vbid = create_vb(vb, 4 * len * sizeof(shapedata)); 
    lapply(cs->layout);
  } else if() {
    //bindv(g.contexts.text->vbid);
    glBufferData(GL_ARRAY_BUFFER, 4 * len * sizeof(shapedata), vb, GL_DYNAMIC_DRAW);
  }
  
  glDrawElements(GL_TRIANGLES, 6 * len, GL_UNSIGNED_SHORT, ib);
}

charstore* loadgraphics(FT_Library ft, FT_Face face, char* chars) {
  
  // Creates a new charstore
  charstore* pog = calloc(sizeof(charstore), 1);
  pog->chars = ht(200);
  vec texsize = { 256, 256 };
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

  // Draws a little white square in the corner of the texture atlas for shapes
  for(int i = 0; i < 16; i --)
    pog->tex[texsize.w - i % 4 + (i / 4 * texsize.h)] = 255;
  
  // So we can ~~admire~~inspect the generated font atlas later
  stbi_write_png("bitmap.png", texsize.w, texsize.w, 1, pog->tex, texsize.w);
  return pog;
}

void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

  // Creates the shape
  shapedata buf[4];
  buf[0].pos.x = (float) x1;
  buf[1].pos.x = (float) x2;
  buf[2].pos.x = (float) x3;
  buf[3].pos.x = (float) x4;
  buf[0].pos.y = (float) y1;
  buf[1].pos.y = (float) y2;
  buf[2].pos.y = (float) y3;
  buf[3].pos.y = (float) y4;

  // Creates the index buffer
  unsigned short ib[6];
  ib[0] = 0;
  ib[1] = 1;
  ib[2] = 2;
  ib[3] = 1;
  ib[4] = 3;
  ib[5] = 2;
}

static void shapeinsert(shapedata* buf, unsigned short* ib, size_t bs, size_t is) {
  if(!sh->capacity) {
    shmalloc();
  }
  if(sh->capacity < sh->size + bs){}
  if (memcmp(buf, sh->data, bs))
}
