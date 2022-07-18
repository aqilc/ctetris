

#include <stdio.h>

// GLEW, for opengl functions
#define GLEW_STATIC
#include <GL/glew.h>

// Window management
#include <GLFW/glfw3.h>


#include <ft2build.h>
#include FT_FREETYPE_H

#include <linmath.h>
#include "util.h"
#include "glapi.h"
#include "graphics.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);


GLFWwindow* window;

gamestate g = { .width = 640, .height = 480 };

bool pressed = false;
pvec mouse;

int main (void) {
  
  /* Initialize the library */
  if (!glfwInit())
    return -1;
  
  // GLFW hints
  //glfwWindowHint(GLFW_DECORATED, GL_FALSE);
  //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
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
  glfwSwapInterval(1);
  
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
  if(FT_New_Face(ft, "res/fonts/iosevka-bold.ttf", 0, &face)) {
    printf("Couldn't load font :(\n"); return -1; }

  // Initializes text shaders and context
  glinitgraphics();
  
  // Loads most of the ASCII chars
  FT_Set_Pixel_Sizes(face, 0, 48);
  charstore* font = loadchars(ft, face, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890[]{}()/\\=+\'\"<>,.-_?|!@#$%^&* :");

  
  // Sets the coordinate system to screen 1:1 pixel mapping
  coords_screen();

  // GLFW input callbacks
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  
  char fps[] = "       ";
  while(!glfwWindowShouldClose(window)) {
    g.delta = glfwGetTime() - g.last;
    glClear(GL_COLOR_BUFFER_BIT);

    // Draws "hello" on the screen
    text(font, "hello :D", 100, 100);

    sprintf(fps, "%.1f", 1.0 / g.delta);
    text(font, fps, 500, 100);

    //quad(200, 200, 300, 200, 200, 300, 300, 300, (vec4) { 1.0f, .5f, .3f, 1.0f });
  
    draw();
    g.last = glfwGetTime();
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
    else { pressed = true; glfwGetCursorPos(window, &mouse.x, &mouse.y); }
  else pressed = false;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
  if(pressed) {
    int wxpos, wypos;
    glfwGetWindowPos(window, &wxpos, &wypos);
    glfwSetWindowPos(window, (int) (xpos - mouse.x) + wxpos, (int) (ypos - mouse.y) + wypos);
  }
}
