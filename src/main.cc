#include <iostream>
#include <string>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "setup.h"
#include "errors.h"
#include "shader/shader.h"
#include "buffer/index_buffer.h"
#include "buffer/vertex_buffer.h"
#include "vertex_array/vertex_array.h"
#include "renderer/renderer.h"
#include "textures/texture.h"

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

  float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 1.0f,
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0,
  };

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLClearError();
  // setup vertex array
  VertexArray va;
  // setup vertex buffer + its layout
  VertexBuffer vbuff{ vertices, 4*4*sizeof(float) };
  BufferLayout layout;
  layout.PushField(2);
  layout.PushField(2);
  va.AddBuffer(vbuff, layout);
  GLCheckError();
  // setup index buffer
  IndexBuffer ib{ indices, 6 };
  // install shaders
  Shader sh{{
    "resources/shaders/basic-vertex-shader.glsl",
    "resources/shaders/basic-fragment-shader.glsl"
  }};
  sh.Bind();
  GLCheckError();
  // setup textures
  Texture t{ "resources/textures/OpenGL_logo.png" };
  t.Bind();
  GLCheckError();

  VectorUniform<int> textureUniform{ 1, 0 };
  sh.setVectorUniform("u_Texture", &textureUniform);
  GLCheckError();

  // loop
  Renderer r;

  while (!glfwWindowShouldClose(window)) {
    r.Clear();
    r.Draw(va, ib, sh);
    GLCheckError();
    // swap buffers and poll for events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup
  glfwDestroyWindow(window);
  glfwTerminate();
}
