#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../logger/logger.h"

class Window {
  GLFWwindow *m_win;
public:
  // we assume GLFW has been initialized by the time that the window is
  // constructed.
  Window(const int width, const int height, const char *title)
    : m_win{ glfwCreateWindow(width, height, title, NULL, NULL) }
  { }

  ~Window() { glfwDestroyWindow(m_win); }

  // Check if an error occured during window creation.
  // If `exitOnErr` is set to true (which it is by default), 
  inline void checkInitErr(bool exitOnErr = true) {
    if (m_win == NULL) {
      Logger log;
      log << LoggerState::Error << "failed to open GLFW window\n";
      glfwTerminate();
    }
  }

  inline void makeCurrent() { glfwMakeContextCurrent(m_win); }
  inline bool shouldClose() { return glfwWindowShouldClose(m_win); }
  inline void swapBuffers() { glfwSwapBuffers(m_win); }

  void setKeyCallback(GLFWkeyfun cb) {
    glfwSetKeyCallback(m_win, cb);
  }
};

#endif
