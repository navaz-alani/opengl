#include <iostream>
#include <string>
#include <cstdlib>
// OpenGL related headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// abstractions
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
#include "model-parser/obj-parser.h"

int main(void) {
  Logger log;

  setupGLFW();
  // Open a window and create its OpenGL context
  Window win{ 1024, 512, "OpenGL" };
  win.checkInitErr();
  win.makeCurrent();
  log << LoggerState::Info << "window setup completed";

  setupGLEW();

  // setup blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  ObjParser parser;
  Object3D model = parser.parse("resources/objects/blender.obj");
  if (parser.isParseError()) {
    log << LoggerState::Error << "object file parse failed";
    log << LoggerState::Error << parser.getParseError();
    return 1;
  }
  const unsigned int numVertices = model.vertexPositions.size();
  log << LoggerState::Info << "object file parsed";

  VertexArray rect;
  VertexBuffer vbuff{
    (float *)model.vertexPositions.data(),
    (unsigned int)(numVertices*sizeof(float))*3
  };
  GLCheckError("vertex buffer");
  log << LoggerState::Info << "vertex buffer setup completed";

  BufferLayout layout;
  layout.PushField(3);
  rect.AddBuffer(vbuff, layout);
  GLCheckError("vertex array binding");
  log << LoggerState::Info << "vertex array setup completed";

  IndexBuffer ib{
    (unsigned int *)model.faces.data(),
    (unsigned int)model.faces.size()*3
  };
  GLCheckError("index buffer");
  log << LoggerState::Info << "index buffer setup completed";

  Shader sh{{
    "resources/shaders/basic-vertex-shader.glsl",
    "resources/shaders/basic-fragment-shader.glsl"
  }};
  GLCheckError("shader");
  log << LoggerState::Info << "shader setup completed";

  glm::mat4 rot = glm::mat4(1.0f);
  rot = glm::translate(rot, glm::vec3(0.0, 0.0, 0.0));
  rot = glm::rotate(rot, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
  rot = glm::rotate(rot, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
  MatrixUniform<glm::mat4> rotMatrix{ "u_Rotation", 4, rot };

  sh.Bind();
  sh.setUniform(&rotMatrix);
  GLCheckError("setting uniform");
  log << LoggerState::Info << "uniforms set";

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
