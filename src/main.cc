#include <iostream>
#include <string>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "setup.h"
#include "errors.h"
#include "shader/shader.h"
#include "buffer/index_buffer.h"
#include "buffer/vertex_buffer.h"
#include "vertex_array/vertex_array.h"
#include "renderer/renderer.h"
#include "textures/texture.h"
#include "chrono/chrono.h"

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
  //glfwSwapInterval(1);
  //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // buffer setup

  float rect1[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
  };
  float rect2[] = {
    -0.25f, -0.25f,
     0.25f, -0.25f,
     0.25f,  0.25f,
    -0.25f,  0.25f
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0,
  };


  // setup blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLClearError();

  // setup vertex arrays
  VertexArray r1, r2;
  // setup vertex buffer
  VertexBuffer vbuff1{ rect1, 4*2*sizeof(float) },
               vbuff2{ rect2, 4*2*sizeof(float) };
  // setup layout
  BufferLayout layout;
  layout.PushField(2);
  // bind the buffers to the separate vertex arrays
  r1.AddBuffer(vbuff1, layout);
  r2.AddBuffer(vbuff2, layout);
  GLCheckError();

  // setup index buffer
  IndexBuffer ib{ indices, 6 };
  // install shaders
  Shader sh{{
    "resources/shaders/solid-color-vertex-shader.glsl",
    "resources/shaders/solid-color-fragment-shader.glsl"
  }};
  GLCheckError();

  // setup color uniforms
  VectorUniform<float> rect1Color{ 4, 1.0f, 0.0f, 1.0f, 1.0f };
  VectorUniform<float> rect2Color{ 4, 1.0f, 1.0f, 1.0f, 0.5f };
  GLCheckError();


  sh.Bind();
  const char *colorUniform = "u_Color";
  sh.setVectorUniform(colorUniform, &rect1Color);

  GLCheckError();

  // loop
  Renderer r;

  unsigned int frameCount{ 0 };
  Chrono timer;

  while (!glfwWindowShouldClose(window)) {
    r.Clear();

    sh.setVectorUniform(colorUniform, &rect1Color);
    GLCheckError();
    r.Draw(r1, ib, sh);
    GLCheckError();

    ++frameCount;

    sh.setVectorUniform(colorUniform, &rect2Color);
    GLCheckError();
    r.Draw(r2, ib, sh);
    GLCheckError();

    // swap buffers and poll for events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  timer.log("application time");
  std::cout << frameCount<< " frames rendered\n";

  // cleanup
  glfwDestroyWindow(window);
  glfwTerminate();
}
