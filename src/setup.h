#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger/logger.h"

int setupGLFW() {
  Logger log;

  if (!glfwInit()) {
    log << LoggerState::Error << "failed to initialize GLFW";
    return -1;
  }
  // 4x antialiasing
  glfwWindowHint(GLFW_SAMPLES, 4);
  // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // To make MacOS happy
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // We don't want the old OpenGL
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glewExperimental=true; // Needed in core profile

  log << LoggerState::Info << "initialized GLFW";
  return 0;
}

int setupGLEW() {
  Logger log;

  if (glewInit() != 0) {
    log << LoggerState::Error << "failed to initialize GLEW";
    return 1;
  }
  glewExperimental=true; // Needed in core profile
  glfwSwapInterval(1);   // match monitor refresh rate
  // Ensure we can capture the escape key being pressed below
  //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  log << LoggerState::Info << "initialized GLEW";
  return 0;
}
