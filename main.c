#include <stdio.h>
#include <math.h>

// GLEW, for opengl functions
#define GLEW_STATIC
#include <GL/glew.h>

// Window management
#include <GLFW/glfw3.h>


#include <ft2build.h>
#include FT_FREETYPE_H

#include "2dgraphics.h"
#include "tetris.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


GLFWwindow* window;

gamestate g = { .width = 640, .height = 480, .frames = 0 };

bool pressed = false;
pvec pressplace;
struct { double x, y; bool pressed; } mouse;

double frameaccum = 0;
char framerate[12];

int main (void) {
  
  /* Initialize the library */
  if (!glfwInit())
    return -1;
  
  // GLFW hints
  glfwWindowHint(GLFW_DECORATED, GL_FALSE);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(g.width, g.height, "Hello World", NULL, NULL);
  if (!window) {
    puts("couldn't initialize window???");
    glfwTerminate(); return -1; }
    
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  // Set the framerate to the framerate of the screen.
  glfwSwapInterval(.5);
  
  // Initialize GLEW so we can reference OpenGL functions.
  if(glewInit()/* != GLEW_OK */) {
    printf("error with initializing glew");
    glfwTerminate(); return -1; }
  
  // Sets the debug message callback so we can detect opengl errors
  glDebugMessageCallback(MessageCallback, NULL);

  // Enables blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  
  // Loads the freetype library and font.
  FT_Library ft; FT_Face face;
  if(FT_Init_FreeType(&ft)) {
    printf("Couldn't init freetype :(\n"); return -1; }
  if(FT_New_Face(ft, "d:/projects/c/ctetris/res/fonts/iosevka-bold.ttf", 0, &face)) {
    printf("Couldn't load font :(\n"); return -1; }

  // Initializes text shaders and context
  glinitgraphics();

  // Loads most of the ASCII chars
  FT_Set_Pixel_Sizes(face, 0, 48);
  charstore* font = loadchars(ft, face, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890[]{}()/\\=+\'\"<>,.-_?|!@#$%^&* :");
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  // Sets the coordinate system to screen 1:1 pixel mapping
  coords_screen();

  // GLFW input callbacks
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetKeyCallback(window, key_callback);

  // Makes a tetris board
  tetrisstate t = {.width = 10, .height = 20};
  tetinit(&t);
  t.x = 100;
  t.y = 50;

  bool textchanged = true;
  while(!glfwWindowShouldClose(window)) {

    // Framerate stuff
    g.delta = glfwGetTime() - g.last;
    g.last = glfwGetTime();
    g.frames ++;
    frameaccum += 1.0/g.delta;

    // If there have been more than 10 accumulated frames, reset the accumulation and update framecount
    if(g.frames % 100 == 0) {
      snprintf(framerate, sizeof(framerate), "%.2f fps", frameaccum/100);
      frameaccum = 0;
      textchanged = true;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    //setu2f("mouse", (vec2) { (float) mouse.x, (float) mouse.y });

    // c(1.0f, .5f, .3f, 1.0f);
    // rect(g.width - 150, g.height - 50, 200, -20);

    // printf("%.4f\n", g.delta);
    if(textchanged) {
      c(.5f, .5f, .5f, 1.0f);
      tsiz(20);
      text(framerate, g.width - 130, g.height - 10);
      textchanged = false;
    } else skip(TEXT, strlen(framerate));

    tetdraw(&t);

    c(1.0f, 1.0f, 1.0f, 1.0f);
    rect(0, 0, g.width, 1);
    rect(0, g.height-1, g.width, 1);
    rect(0, 0, 1, g.height);
    rect(g.width-1, 0, 1, g.height);

    draw();
    //glfwSetWindowShouldClose(window, 1);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (action == GLFW_PRESS)
    if(button == GLFW_MOUSE_BUTTON_RIGHT)
      glfwSetWindowShouldClose(window, 1);
    else { pressed = true; glfwGetCursorPos(window, &pressplace.x, &pressplace.y); }
  else pressed = false;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
  mouse.x = xpos;
  mouse.y = ypos;
  if(pressed) {
    int wxpos, wypos;
    glfwGetWindowPos(window, &wxpos, &wypos);
    glfwSetWindowPos(window, (int) (xpos - pressplace.x) + wxpos, (int) (ypos - pressplace.y) + wypos);
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  
}
