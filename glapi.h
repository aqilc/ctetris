
#ifndef GLAPI_H
#define GLAPI_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <linmath.h>
#include "util.h"

struct ltype {
  GLuint type;
  unsigned char count;
  bool normalized;
  struct ltype* next;
};
typedef struct vlayout {
  struct ltype* types;
  unsigned short stride;
} vlayout;

GLuint* create_va(char n);
GLuint create_vb(void* data, unsigned int size);
GLuint create_ib(short* data, unsigned int size);

void binda(GLuint id);
void bindv(GLuint id);
void bindi(GLuint id);
void bindp(GLuint id);

void activet(GLuint id, char slot);
void bindt(GLuint id);
GLuint texture(unsigned char* buf, int width, int height, GLenum format);


void lpush(vlayout* l, GLuint type, unsigned char count, bool normalized);
void lpushf(vlayout* l, unsigned char count);
void lpushc(vlayout* l, unsigned char count);
void lpushi(vlayout* l, unsigned char count);
void lapply(vlayout* l);

GLint getloc(char* name);

void setum4(char* name, mat4x4 p);
void setui(char* name, unsigned int p);
void setuf(char* name, vec3 p);

void context(drawcontext* c);
void initcontext(drawcontext** d, char* shaderfile);

void coords_screen(void);

unsigned int compileshader(unsigned int type, char* src);
unsigned int create_p(char* filename);

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif

