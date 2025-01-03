
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "tetris.h"

// All of the variables with the tetris data and mappings :D
#include "tetdata.h"


static char* newqueue(void);
static void spawn(tetrisstate* s);
static void place(tetrisstate* s);
static inline char gp(TET_BOARD_T x, TET_BOARD_T y);
static inline char gpv(vect c);
static char next(unsigned int n);

// Current board pointer, so we can emit a param from certain functions, like `gp`
static tetrisstate* cur = NULL;

tetrisstate* tetinit(tetrisstate* s) {

	// Initializes the piece lookup table
	if(!pmap[1]) {
		pmap[0] = (char)0; // I
		pmap[1] = (char)3; // J
		pmap[3] = (char)2; // L
		pmap[6] = (char)6; // O
		pmap[9] = (char)4; // S
		pmap[10] = (char)1; // T
		pmap[17] = (char)5; // Z
	}

	// If it's not been full yet, initialize it
	if(!s->buf) s->buf = malloc(s->width * (s->height + TET_BUFFER_ROWS));
	
	// Initializes all bytes
	for (int i = 0; i < s->width * (s->height + TET_BUFFER_ROWS); i++)
		s->buf[i] = 0;

	// IDK everything else now is 0
	s->px = 0;
	s->py = 0;
	s->rot = 0;
	s->piece = '\0';

	// Sets up buffer for queue
	if(!s->qbuf)
		s->qbuf = malloc(20), s->qblen = 20;
	s->qlen = 0;
	s->qsp = 0;

	if(!s->bsize.x) s->bsize.x = TET_BSIZE, s->bsize.y = TET_BSIZE;

	return s;
}

void tetdraw(tetrisstate* board) {
	if(board->piece == '\0') spawn(board);
	cur = board;

	int bx = board->x;
	int by = board->y;
	int bw = board->width;
	int bh = board->height;
	int blx = board->bsize.x;
	int bly = board->bsize.y;
	
	fill(0.f, 0.f, 0.f, 1.f);
  rect(bx, by, bw * blx, bh * bly);

  fill(1.f, 1.f, 1.f, 1.f);
  rect(bx - 2, by, 2, bh * bly);
  rect(bx + bw * blx - 2, by, 2, bh * bly);
  rect(bx, by + bh * bly - 2, bw * blx, 2);

	for (int i = 0; i < bw * (bh + TET_BUFFER_ROWS); i ++) {
		if(!board->buf[i]) continue;
    float* col = getcol(board->buf[i]);
    fill(col[0], col[1], col[2], 1.f);

    // Calculates x and y based on just the i value
    int x = i % bw * blx + bx;
    int y = (i / bw) * bly + by;

    // Base block
    rect(x, y, blx, bly);

    // Draws an outline around the block
    fill(col[0] + .196f, col[1] + .196f, col[2] + .196f, 1.f);
    rect(x, y, 1, bly);
    rect(x + blx - 1, y, 1, bly);
    rect(x, y, blx, 1);
    rect(x, y + bly - 1, blx, 1);
  }

	drawblock(board, board->piece, board->rot, board->px * blx + bx, board->py * bly + by);

	// Draws the queue
	for(int i = 0; i < 5; i ++)
		drawblock(board, next(i), 0, bx + bw * blx + 20, by + 10 + i * 50);

	
}

static void drawblock(tetrisstate* board, char piece, int rot, int bx, int by) {
	vect* b = (vect*) (((block*) getp(piece)) + rot);
	float* col = getcol(piece);
	for(int i = 0; i < 4; i++) {
    int x = bx + b[i].x * board->bsize.x;
    int y = by + b[i].y * board->bsize.y;
		fill(col[0], col[1], col[2], 1.f);
		rect(x, y, board->bsize.x, board->bsize.y);
    fill(col[0] + .196f, col[1] + .196f, col[2] + .196f, 1.f);
    rect(x, y, 1, board->bsize.y);
    rect(x + board->bsize.x - 1, y, 1, board->bsize.y);
    rect(x, y, board->bsize.x, 1);
    rect(x, y + board->bsize.y - 1, board->bsize.x, 1);
	}
}

// TODO
void tetstep(tetrisstate* s, double time) {}

// Returns a shuffled queue, **7** elements
static char* newqueue() {
	char* q = malloc(7);
	memcpy(q, (char[]) {'I', 'L', 'J', 'Z', 'S', 'T', 'O'}, 7);

	// Scrambles everything
	for (char i = 6; i > 0; i --) {
		char ez = rand() % i;
		char t = q[ez];
		q[ez] = q[i];
		q[i] = q[ez];
	}

	return q;
}

// Spawns a piece into the game
static void spawn(tetrisstate* s) {
	if (s->qlen <= 7) {
		char* q = newqueue();

		// Copies the queue over, following stack ptr rules (PLS DON'T SEGFAULT)
		for(char i = 0; i < 7; i ++) {
			s->qbuf[(s->qlen + s->qsp) % s->qblen] = q[i];
			s->qlen ++;
		}

		free(q);
	}

	// Gets a new piece from queue
	s->piece = s->qbuf[s->qsp ++];
	s->qsp = s->qsp % s->qblen;

	// Sets the piece coords
	if(s->piece == 'O') s->px = 4;
	else s->px = 3;

	s->py = 0;
	s->rot = 0;
	s->bottom = 0.0;
}

static char next(unsigned int n) {
	return cur->qbuf[(cur->qsp + n) % cur->qblen];
}

// Places the piece into the board exactly where it is at relatively
static void place(tetrisstate* s) {
	vect* p = (vect *)((block *) getp(s->piece) + s->rot);
	for(int i = 0; i < 4; i ++) {
		int x = p[i].x + s->px;
    int y = p[i].y + s->py + TET_BUFFER_ROWS;
    if(x < 0 || x > s->width || y < 0 || y > s->height) return;
    s->buf[x + y * s->width] = s->piece;
	}
}

static void collide(tetrisstate* s, piece* p, int px, int py, int rot) {

	for(int i = 0; i < 4; i ++) {}
}

void tet_free(tetrisstate* s) { free(s->buf); free(s->qbuf); };

// So I don't make a dumb typo :(
static inline char gp(TET_BOARD_T x, TET_BOARD_T y) { return cur->buf[x + y * cur->width]; }
static inline char gpv(vect c) { return cur->buf[c.x + c.y * cur->width]; }

