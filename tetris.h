
#include "g/2dgraphics.h"


/*
  Board buffer format:
    0: nothing
    '[char]': Letter
    everything else: garbage
*/

#define TET_BOARD_T char
#define TET_BUFFER_ROWS 3
#define TET_BSIZE 20
typedef struct vect { TET_BOARD_T x, y; } vect;
typedef struct tetrisstate {
  char* buf;

  TET_BOARD_T width;
  TET_BOARD_T height;
  vect bsize;

  // How the board is drawn
  int x, y, bs /* block size */;

  // Piece things
  char piece;
  int px;
  int py;
  int rot;
  double bottom;

  // Queue
  char* qbuf;
  char qsp; // Queue Stack Pointer
  char qlen;
  char qblen; // Buffer len

  void (*keyboard)(void);
} tetrisstate;

typedef vect block[4];
typedef block piece[4];

tetrisstate* tetinit(tetrisstate* s);
void tetdraw(tetrisstate* board);
static void drawblock(tetrisstate* board, char piece, int rot, int bx, int by);
void tetstep(tetrisstate* s, double time);
void tet_free(tetrisstate* s);

