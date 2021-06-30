#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../logger/logger.h"
#include "input_controller.h"

class Window {
  GLFWwindow      *m_win;
  InputController *m_ic;
public:
  // we assume GLFW has been initialized by the time that the window is
  // constructed.
  Window(const int width, const int height, const char *title)
    : m_win{ glfwCreateWindow(width, height, title, NULL, NULL) }
  { }

  ~Window() { glfwDestroyWindow(m_win); }

  // Check if an error occurred during window creation.
  // If `exitOnErr` is set to true (which it is by default), the program will be
  // exited if an error occurred.
  inline void checkInitErr(bool exitOnErr = true) {
    if (m_win == NULL) {
      Logger log;
      log << LoggerState::Error << "failed to open GLFW window\n";
      glfwTerminate();
      if (exitOnErr) exit(1);
    }
  }

  inline void makeCurrent() { glfwMakeContextCurrent(m_win); }
  inline bool shouldClose() { return glfwWindowShouldClose(m_win); }
  inline void swapBuffers() { glfwSwapBuffers(m_win); }

  void setInputController(InputController *ic) {
    m_ic = ic;
    // setup controller to manage input events for the window
    glfwSetInputMode(m_win, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
    glfwSetInputMode(m_win, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetKeyCallback(m_win, ic->keyCallback);
    glfwSetCharCallback(m_win, ic->charCallback);
  }
};

#endif
