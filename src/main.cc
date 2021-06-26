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
#include "shader/vector_uniform.h"
#include "shader/matrix_uniform.h"
#include "buffer/index_buffer.h"
#include "buffer/vertex_buffer.h"
#include "vertex_array/vertex_array.h"
#include "renderer/renderer.h"
#include "textures/texture.h"
#include "chrono/chrono.h"
#include "logger/logger.h"
#include "window.h"

int main(void) {
  Logger log;

  setupGLFW();
  // Open a window and create its OpenGL context
  Window win{ 1024, 512, "OpenGL" };
  win.checkInitErr();
  win.makeCurrent();
  log << LoggerState::Info << "window setup completed";

  setupGLEW();

  // buffer setup
  float rect1[] = {
    -0.5f, -0.5f,  0.0f,  0.0f,
     0.5f, -0.5f,  1.0f,  0.0f,
     0.5f,  0.5f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.0f,  1.0f
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
  VertexArray rect;
  // setup vertex buffer
  VertexBuffer vbuff1{ rect1, 4*4*sizeof(float) };
  // setup layout
  BufferLayout layout;
  layout.PushField(2);
  layout.PushField(2);
  // bind the buffers to the separate vertex arrays
  rect.AddBuffer(vbuff1, layout);
  GLCheckError("vertex layout setup");

  // setup index buffer
  IndexBuffer ib{ indices, 6 };

  // install shaders
  Shader sh{{
    "resources/shaders/basic-vertex-shader.glsl",
    "resources/shaders/basic-fragment-shader.glsl"
  }};
  GLCheckError("shader compilation and program creation");

  Texture t{ "resources/textures/OpenGL_logo.png" };
  t.Bind();
  GLCheckError("texture init and binding");

  sh.Bind();

  glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
  MatrixUniform<glm::mat4> mvp{ 4, proj };
  sh.setUniform("u_MVP", mvp);

  VectorUniform<int> tex{ 1, 0 };
  sh.setUniform("u_Texture", tex);
  GLCheckError("setting uniforms");

  // loop
  Renderer r;

  unsigned int frameCount{ 0 };
  Chrono timer;

  while (!win.shouldClose()) {
    r.Clear();
    r.Draw(rect, ib, sh);

    GLCheckError("draw call error");

    ++frameCount;
    // swap buffers and poll for events
    win.swapBuffers();
    glfwPollEvents();
  }

  timer.log("application time");
  std::cout << frameCount<< " frames rendered\n";

  log << LoggerState::Info << "exiting application";

  // cleanup
  glfwTerminate();
}
