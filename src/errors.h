#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <iostream>
#include <string>
#include <GL/glew.h>

inline void GLClearError() { while (glGetError() != GL_NO_ERROR); }

inline void GLCheckError() {
  while (GLenum err = glGetError()) {
    std::cerr << "[OpenGL Error] (code 0x"
              << std::hex << err << ") "
              << gluErrorString(err)
              << "\n";
  }
}

// a context string describes the context where the error occurred.
inline void GLCheckError(const std::string &context) {
  bool hasErrored = false;
  while (GLenum err = glGetError()) {
    if (!hasErrored && (hasErrored = true))
      std::cerr << "Error(s) in context: '" << context << "\n";
    std::cerr << "[OpenGL Error] (code 0x"
              << std::hex << err << ", ctx:"
              << context << ") "
              << gluErrorString(err)
              << "\n";
  }
  hasErrored && std::cerr << "DONE\n";
}

#endif
