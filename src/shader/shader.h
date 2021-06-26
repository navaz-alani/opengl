#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>

#include "../errors.h"
#include "loader.h"
#include "uniform.h"

class Shader {
  unsigned int m_glID;
public:
  Shader(const ShaderSource src);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  void setUniform(const char *name, Uniform &u) {
    // TODO: uniform location cache

    int location = glGetUniformLocation(m_glID, name);
    if (location == -1) return;
    u.setToLocation(location);
  }
};

#endif
