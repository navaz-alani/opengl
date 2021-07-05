#include <iostream>
#include <string>
#include <cstdlib>
#include <functional>
// OpenGL related headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "setup.h"
#include "model-renderer/renderer.h"

int main(void) {
  Logger log;

  setupGLFW();
  // Open a window and create its OpenGL context
  Window win{ 512, 512, "OpenGL" };
  win.checkInitErr();
  win.makeCurrent();
  log << LoggerState::Info << "window setup completed\n";

  setupGLEW();
  // setup blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // setup depth test
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  const std::string objFile = "resources/objects/teapot.obj";
  ModelRenderer renderer{ &win, objFile };
  renderer.render();
  // cleanup
  glfwTerminate();
}
