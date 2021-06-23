#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <iostream>
#include <string>
#include <cstdlib>

#include <GLFW/glfw3.h>
/*
#include <glm/glm.hpp>
using namespace glm;
*/

#include "setup.h"
#include "shader/shader.h"
#include "buffer/index_buffer.h"
#include "buffer/vertex_buffer.h"
#include "vertex_array/vertex_array.h"
#include "renderer/renderer.h"

void GLClearError() { while (glGetError() != GL_NO_ERROR); }

void GLCheckError() {
  while (GLenum err = glGetError()) {
    std::cerr << "[OpenGL Error] (code 0x"
              << std::hex << err << ") "
              << gluErrorString(err)
              << "\n";
  }
}

int main(void) {
  if (setup() != 0) {
    std::cerr << "Setup failed - exiting";
    return 1;
  }

  // Open a window and create its OpenGL context

  GLFWwindow* window;
  window = glfwCreateWindow( 1024, 768, "OpenGL", NULL, NULL);
  if (window == NULL){
    std::cerr << "Failed to open GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW\n";
    return -1;
  }
  glewExperimental=true; // Needed in core profile
  // Ensure we can capture the escape key being pressed below
  glfwSwapInterval(1);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // buffer setup

  float positions[8] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f,
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0,
  };

  // setup vertex array
  VertexArray va;
  // setup vertex buffer + its layout
  VertexBuffer vbuff{ positions, 4*2*sizeof(float) };
  BufferLayout layout;
  layout.PushField(2);
  va.AddBuffer(vbuff, layout);
  // setup index buffer
  IndexBuffer ib{ indices, 6 };
  // install shaders
  Shader sh{{
    "resources/shaders/basic-vertex-shader.glsl",
    "resources/shaders/basic-fragment-shader.glsl"
  }};

  // loop
  Renderer r;

  while (!glfwWindowShouldClose(window)) {
    r.Clear();
    r.Draw(va, ib, sh);
    // swap buffers and poll for events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup
  glfwDestroyWindow(window);
  glfwTerminate();
}
