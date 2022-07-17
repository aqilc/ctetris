// compile command: gcc texpack.c -o texpack.exe

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

typedef struct vec { int x; int y; } vec;
typedef struct CharNode {
  // Place
  vec p;
  
  // Size
  vec s;
  
  struct CharNode* child[2];
  bool filled;
} CharNode;
CharNode* charnode_insert(CharNode* pnode, vec* size);
void fill(char* buf, int x, int y, int w, int h, char c);

#define BUFH 100
#define BUFW 100

int main() {
  
  time_t t;
  srand((unsigned) time(&t));
  
  char* buff = malloc(sizeof(char) * BUFH * BUFW);
  memset(buff, '_', 10000);
  
  CharNode ez = { .p = { 0, 0 }, .s = { BUFW, BUFH } };
  
  for (int i = 0; i < 80; i ++) {
    vec randsiz = { rand()%18 + 2, rand()%18 + 2 };
    CharNode* res = charnode_insert(&ez, &randsiz);
    if (res == NULL) {printf("break on %d\n", i); break; }
    //printf("filling in %d, %d, %d, %d\n", res->p.x, res->p.y, res->s.x, res->s.y);
    fill(buff, res->p.x, res->p.y, res->s.x, res->s.y, 'a' + i);
  }
  
  //printf(buff);
  char* bruh = malloc(sizeof(char) * 102);
  bruh[100] = '\n';
  bruh[101] = '\0';
  for(int i = 0; i < BUFH; i++)
    printf(strncpy(bruh, buff + i * BUFW, BUFW));
  
  return 0;
}

void fill(char* buf, int x, int y, int w, int h, char c) {
  for(int i = 0; i < h; i ++)
    for(int j = 0; j < w; j ++) {
      buf[x + (i + y) * BUFH + j] = c;
    }
}


CharNode* charnode_insert(CharNode* pnode, vec* size) {
  if(pnode->child[0] == NULL || pnode->child[1] == NULL) {
    
    // If the node is already filled
    if(pnode->filled) return NULL;
    
    // If the image doesn't fit in the node
    if(pnode->s.x < size->x || pnode->s.y < size->y) return NULL;
    
    // If the image fits perfectly in the node, return the node since we can't really split anymore anyways
    if(pnode->s.x == size->x && pnode->s.y == size->y) { pnode->filled = true; return pnode; }
    
		// Splits node
		CharNode* c1 = calloc(1, sizeof(CharNode));
		CharNode* c2 = calloc(1, sizeof(CharNode));
		pnode->child[0] = c1;
		pnode->child[1] = c2;
		
		int dw = pnode->s.x - size->x;
		int dh = pnode->s.y - size->y;
		
		// Sets everything that would be the same for both type of splits
		c1->p.x = pnode->p.x;
		c1->p.y = pnode->p.y;

		// If the margin between the right of the rect and the edge of the parent rectangle is larger than the margin of the bottom of the rect and the bottom of the parent node
		if(dw > dh) {
      // Vertical Split
      c1->s.x = size->x; // - 1
      c1->s.y = pnode->s.y;
      
      c2->p.x = pnode->p.x + size->x;
      c2->p.y = pnode->p.y;
      c2->s.x = pnode->s.x - size->x;
      c2->s.y = pnode->s.y;
      
		} else {
      // Horizontal Split
      c1->s.x = pnode->s.x;
      c1->s.y = size->y; // - 1
      
      c2->p.x = pnode->p.x;
      c2->p.y = pnode->p.y + size->y;
      c2->s.x = pnode->s.x;
      c2->s.y = pnode->s.y - size->y;
    }
    
    return charnode_insert(c1, size);
  }
  
  CharNode* firstchildres = charnode_insert(pnode->child[0], size);
  if(firstchildres == NULL)
    return charnode_insert(pnode->child[1], size);
  return firstchildres;
}

