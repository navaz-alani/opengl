#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger/logger.h"

#define __GLFW_CONTEXT_VERSION_MAJOR__ 4
#define __GLFW_CONTEXT_VERSION_MINOR__ 1

#if (__GLFW_CONTEXT_VERSION_MAJOR__ >= 4) && (__GLFW_CONTEXT_VERSION_MINOR__ >= 3)
void GLAPIENTRY glDebugOutput(
  GLenum source, 
  GLenum type, 
  unsigned int id, 
  GLenum severity, 
  GLsizei length, 
  const char *message, 
  const void *userParam) {
  // ignore non-significant error/warning codes
  if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

  std::cout << "---------------" << std::endl;
  std::cout << "Debug message (" << id << "): " <<  message << std::endl;

  switch (source)
  {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
  } std::cout << std::endl;

  switch (type)
  {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
  } std::cout << std::endl;

  switch (severity)
  {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
  } std::cout << std::endl;
  std::cout << std::endl;
}
#endif

int setupGLFW() {
  Logger log;

  if (!glfwInit()) {
    log << LoggerState::Error << "failed to initialize GLFW\n";
    return -1;
  }
  // 4x antialiasing
  glfwWindowHint(GLFW_SAMPLES, 4);
  // We want OpenGL 4.1
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, __GLFW_CONTEXT_VERSION_MAJOR__); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, __GLFW_CONTEXT_VERSION_MINOR__);
  // To make MacOS happy
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // We don't want the old OpenGL
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#if (__GLFW_CONTEXT_VERSION_MAJOR__ >= 4) && (__GLFW_CONTEXT_VERSION_MINOR__ >= 3)
  // debug context
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    log << LoggerState::Info << "initialized GLFW debug context\n";
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  } else {
    log << LoggerState::Error << "failed to initialize GLFW debug context\n";
  }
#endif

  glewExperimental=true; // Needed in core profile

  log << LoggerState::Info << "initialized GLFW\n";
  return 0;
}

int setupGLEW() {
  Logger log;

  if (glewInit() != 0) {
    log << LoggerState::Error << "failed to initialize GLEW\n";
    return 1;
  }
  glewExperimental=true; // Needed in core profile
  glfwSwapInterval(1);   // match monitor refresh rate
  // Ensure we can capture the escape key being pressed below
  //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  log << LoggerState::Info << "initialized GLEW\n";

  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION);   // version as a string
  log << LoggerState::Info << "Renderer: " << (const char *)renderer << "\n";
  log << LoggerState::Info << "OpenGL version supported " << (const char *)version << "\n";

  return 0;
}
