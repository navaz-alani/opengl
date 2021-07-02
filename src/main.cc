#include <iostream>
#include <string>
#include <cstdlib>
#include <functional>
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
#include "window/window.h"
#include "models/obj-parser.h"
#include "models/object.h"

struct AppCtx {
  Shader    &sh;
  float      z_rot;
  float      dz_rot;
  glm::mat4  vp;
};

int main(void) {
  Logger log;

  setupGLFW();
  // Open a window and create its OpenGL context
  Window win{ 512, 512, "OpenGL" };
  win.checkInitErr();
  win.makeCurrent();
  log << LoggerState::Info << "window setup completed";

  setupGLEW();
  // setup blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // setup depth test
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  ObjParser parser;
  Object3D model = parser.parse("resources/objects/teapot.obj");
  if (parser.isParseError()) {
    log << LoggerState::Error << "object file parse failed";
    log << LoggerState::Error << parser.getParseError();
    return 1;
  }
  log << LoggerState::Info << "object file parsed";
  model.computeNormals();
  log << LoggerState::Info << "object normals computed";

  std::vector<AggVertex_pos_tex_nor> aggregatedVertices = aggregatePosTexNor(model);

  VertexArray rect; rect.Bind();
  VertexBuffer vbuff{
    (float *)aggregatedVertices.data(),
    (unsigned int)(aggregatedVertices.size() * sizeof(AggVertex_pos_tex_nor))
  }; vbuff.Bind();

  GLCheckError("vertex buffer");
  log << LoggerState::Info << "vertex buffer setup completed";

  BufferLayout layout;
  layout.PushField(3);
  layout.PushField(2);
  layout.PushField(3);

  rect.AddBuffer(vbuff, layout, 0);
  GLCheckError("vertex array binding");
  log << LoggerState::Info << "vertex array setup completed";

  Shader sh{{
    "resources/shaders/basic-vertex-shader.glsl",
    "resources/shaders/basic-fragment-shader.glsl"
  }}; sh.Bind();
  GLCheckError("shader");
  log << LoggerState::Info << "shader setup completed";

  float z_rot = 0.0f;
  float dz_rot = 1.0f;
  glm::mat4 proj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 50.0f);
  //glm::mat4 proj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
  glm::mat4 view = glm::lookAt(
    glm::vec3(0,4,10), // Camera is at (4,3,3), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
  );
  glm::mat4 vp = proj * view;
  // update rotation matrix to make object spin
  auto mvp = glm::rotate(vp, glm::radians(z_rot), glm::vec3(0.0, 1.0, 0.0));
  MatrixUniform<glm::mat4> u_MVP{ "u_MVP", 4, mvp };
  sh.Bind();
  sh.setUniform(&u_MVP);
  GLCheckError("uniform updates");

  AppCtx ctx { sh, 0.0f, 1.0f, vp };

  KeyEventHandler arrowKeysHandler =
    [](void *context, int key, int scancode, int action, int mods) {
      AppCtx *ctx = (AppCtx *)context;
      if (key == KeyRight)     ctx->z_rot += ctx->dz_rot;
      else if (key == KeyLeft) ctx->z_rot -= ctx->dz_rot;
      else return;
      auto mvp = glm::rotate(ctx->vp, glm::radians(ctx->z_rot), glm::vec3(0.0, 1.0, 0.0));
      MatrixUniform<glm::mat4> u_MVP{ "u_MVP", 4, mvp };
      ctx->sh.Bind();
      ctx->sh.setUniform(&u_MVP);
      GLCheckError("uniform updates");
    };
  // setup input controller
  InputController ic;
  ic.setHandleKeyEvents(true); ic.Bind();
  ic.addKeyEventHandler(arrowKeysHandler, (void *)(&ctx));
  // attach input controller to window
  win.setInputController(&ic);

  //sh.setUniforms({ &u_MVP , &u_Texture });
  GLCheckError("setting uniform");
  log << LoggerState::Info << "uniforms set";

  // loop
  Renderer r;
  unsigned int frameCount{ 0 };
  Chrono timer;

  while (!win.shouldClose()) {
    r.Clear();
    r.Draw(rect, aggregatedVertices.size(), sh);
    GLCheckError("draw call error");

    ++frameCount;
    // swap buffers and poll for events
    win.swapBuffers();
    glfwWaitEvents();
  }

  timer.log("application time");
  std::cout << frameCount<< " frames rendered\n";

  log << LoggerState::Info << "exiting application";

  // cleanup
  glfwTerminate();
}
